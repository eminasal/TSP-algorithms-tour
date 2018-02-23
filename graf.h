#ifndef GRAF_H
#define GRAF_H
#include "grad.h"
#include "binarni.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <climits>
#include <algorithm>
#include <cmath>

using namespace std;

class Graf {

    struct Grana {
        int tezina;
        int pocetak;
        int kraj;
        Grana(int tezina, int pocetak, int kraj)
            : tezina(tezina)
            , pocetak(pocetak)
            , kraj(kraj)
        {
        }
        bool operator<(Grana druga) { return tezina < druga.tezina; }
    };

    int n;
    vector<Grad> gradovi;
    vector<vector<int> > udaljenosti;
    vector<Grana> grane;

    void ispisiTuru(vector<int>& tura);
    bool provjeriTuru(vector<int>& tura);
    int udaljenost(double x1, double y1, double x2, double y2)
    {
        return round(sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
    }

public:
    int duzinaTure(vector<int>& tura);
    Graf(string filename);
    int turaNajbliziSusjed();
    int turaNajmanjaGrana();
    vector<int> NajbliziSusjed();   //VRACA VEKTOR TURU
    vector<int> NajmanjaGrana();    //VRACA VEKTOR TURU
    void zamjenaGrana(vector<int>& grane, int& i, int& j);  //VRSI ZAMJENU GRANA
    int turaDvaOpt(int i);     //VRSI 2OPT ALGORITAM NA NEKOJ TURI
};

Graf::Graf(string filename)
{
    ifstream unos(filename);
    while (!unos.eof()) {
        int indeks;
        double latitude;
        double longitude;
        unos >> indeks;
        unos.get();
        unos >> latitude;
        unos.get();
        unos >> longitude;
        unos.get();
        gradovi.push_back(Grad(indeks, latitude, longitude));
    }
    unos.close();
    n = gradovi.size();
    udaljenosti.resize(n);
    for (int i = 0; i < n; i++)
        udaljenosti[i].resize(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            udaljenosti[i][j] = udaljenost(gradovi[i].latitude, gradovi[i].longitude, gradovi[j].latitude, gradovi[j].longitude);
            if (i < j)
                grane.push_back(Grana(udaljenosti[i][j], i, j));
        }
}

int Graf::duzinaTure(vector<int>& tura)
{
    int duzina_ture = 0;
    for (int i = 0; i < tura.size(); i++)
        duzina_ture += udaljenosti[tura[i]][tura[(i + 1) % n]];
    return duzina_ture;
}

void Graf::ispisiTuru(vector<int>& tura)
{
    for (int i = 0; i < tura.size(); i++)
        cout << tura[i] << " ";
    cout << endl;
}

bool Graf::provjeriTuru(vector<int>& tura)
{
    vector<bool> posjecen(n, false);
    if (tura.size() != n)
        return false;
    else {
        for (int i = 0; i < n; i++) {
            if (tura[i] < 0 || tura[i] >= n || posjecen[tura[i]] == true)
                return false;
            else
                posjecen[tura[i]] = true;
        }
    }
    return true;
}

int Graf::turaNajbliziSusjed()
{
    vector<int> tura;
    int pocetni = 0;
    vector<bool> posjecen(n, false);
    posjecen[pocetni] = true;
    tura.push_back(pocetni);
    for (int i = 0; i < n - 1; i++) {
        int trenutni = tura[tura.size() - 1];
        int udaljenostNajblizeg = INT_MAX;
        int najblizi;
        for (int j = 0; j < n; j++) {
            if (!posjecen[j] && udaljenosti[trenutni][j] < udaljenostNajblizeg) {
                udaljenostNajblizeg = udaljenosti[trenutni][j];
                najblizi = j;
            }
        }
        posjecen[najblizi] = true;
        tura.push_back(najblizi);
    }
    if (provjeriTuru(tura))
        return duzinaTure(tura);
    else
        throw "Greska";
}

vector<int> Graf::NajbliziSusjed()
{
    vector<int> tura;
    int pocetni = 0;
    vector<bool> posjecen(n, false);
    posjecen[pocetni] = true;
    tura.push_back(pocetni);
    for (int i = 0; i < n - 1; i++) {
        int trenutni = tura[tura.size() - 1];
        int udaljenostNajblizeg = INT_MAX;
        int najblizi;
        for (int j = 0; j < n; j++) {
            if (!posjecen[j] && udaljenosti[trenutni][j] < udaljenostNajblizeg) {
                udaljenostNajblizeg = udaljenosti[trenutni][j];
                najblizi = j;
            }
        }
        posjecen[najblizi] = true;
        tura.push_back(najblizi);
    }
    if (provjeriTuru(tura))
        return tura;
    else
        throw "Greska";
}

int Graf::turaNajmanjaGrana()
{
    Ruta ruta(n);
    sort(grane.begin(), grane.end());
    vector<int> stepeni(n, 0);
    int trenutna = 0;
    for (int i = 0; i < n - 1; i++) {
        while (true) {
            Grana trenutnaGrana = grane[trenutna];
            int pocetak = trenutnaGrana.pocetak;
            int kraj = trenutnaGrana.kraj;
            if (stepeni[pocetak] == 2 || stepeni[kraj] == 2) {
                trenutna++;
                continue;
            }
            ruta.dodajGranu(pocetak, kraj);
            if (ruta.imaLiCiklusa(pocetak)) {
                ruta.obrisiGranu(pocetak, kraj);
                trenutna++;
                continue;
            }
            else {
                stepeni[pocetak]++;
                stepeni[kraj]++;
                trenutna++;
                break;
            }
        }
    }

    vector<int> tura = ruta.vratiTuru();
    if (provjeriTuru(tura))
        return duzinaTure(tura);
    else
        throw "Greska";
}

vector<int> Graf::NajmanjaGrana()
{
    Ruta ruta(n);
    sort(grane.begin(), grane.end());
    vector<int> stepeni(n, 0);
    int trenutna = 0;
    for (int i = 0; i < n - 1; i++) {
        while (true) {
            Grana trenutnaGrana = grane[trenutna];
            int pocetak = trenutnaGrana.pocetak;
            int kraj = trenutnaGrana.kraj;
            if (stepeni[pocetak] == 2 || stepeni[kraj] == 2) {
                trenutna++;
                continue;
            }
            ruta.dodajGranu(pocetak, kraj);
            if (ruta.imaLiCiklusa(pocetak)) {
                ruta.obrisiGranu(pocetak, kraj);
                trenutna++;
                continue;
            }
            else {
                stepeni[pocetak]++;
                stepeni[kraj]++;
                trenutna++;
                break;
            }
        }
    }

    vector<int> tura = ruta.vratiTuru();
    if (provjeriTuru(tura))
        return tura;
    else
        throw "Greska";
}

//////////////////////////////////////////////////////////// ZAMJENA GRANA ZA 2-OPT ALGORITAM

void Graf::zamjenaGrana(vector<int>& grane, int& i, int& j)
{
    int mini = min(i, j);
    int maxi = max(i, j);

    if (maxi - mini > 2) {
        stack<int> privremeni;
        for (int i = mini + 1; i < maxi; i++)
            privremeni.push(grane[i]);

        int t = mini + 1;
        while (!privremeni.empty()) {
            grane[t] = privremeni.top();
            privremeni.pop();
            t++;
        }
    }
    int t = grane[mini];
    grane[mini] = grane[maxi];
    grane[maxi] = t;
}

/////////////////////////////////////////////////// 2-OPT ALGORITAM

int Graf::turaDvaOpt(int i)
{
    vector<int> optimiziranaTura;
    if (i == 1)
        optimiziranaTura = NajbliziSusjed();
    else if (i == 2)
        optimiziranaTura = NajmanjaGrana();
    else
        throw "greska u izboru ture!";

    bool izvrsavanje = true;

    vector<int> izmjenjenaTura = optimiziranaTura;

    while (izvrsavanje) {
        izvrsavanje = false;
        int razdaljina = duzinaTure(optimiziranaTura);
        for (int i = 0; i < n - 1; i++) {
            for (int j = i + 1; j < n; j++) {
                zamjenaGrana(izmjenjenaTura, i, j);

                if (duzinaTure(izmjenjenaTura) < duzinaTure(optimiziranaTura)) {
                    optimiziranaTura = izmjenjenaTura;
                    izvrsavanje = true;
                }
                else
                    izmjenjenaTura = optimiziranaTura;
            }
        }
    }
    return duzinaTure(optimiziranaTura);
}

#endif // GRAF_H
