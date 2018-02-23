#include <iostream>
#include "graf.h"
#include <string>
#include <ctime>
#include <cstdio>
#include <chrono>

using namespace std;

int main()
{

    string filename("qatar.txt");
    Graf G(filename);

    auto start = std::chrono::high_resolution_clock::now();
    cout<<"Tura najblizi susjed"<<endl<<"Duzina ture: ";
    cout << G.turaNajbliziSusjed()<<endl;
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Vrijeme izvrsavanja:  " << elapsed.count() << " s\n"<<endl;

    cout<<"Tura najmanja grana"<<endl<<"Duzina ture: ";
    auto start2 = std::chrono::high_resolution_clock::now();
    cout << G.turaNajmanjaGrana()<<endl;
    auto finish2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed2 = finish2 - start2;
    std::cout << "Vrijeme izvrsavanja:  " << elapsed2.count() << " s\n"<<endl;

    cout<<"Tura najblizi susjed + 2-opt"<<endl<<"Duzina ture: ";
    auto start3 = std::chrono::high_resolution_clock::now();
    cout<<G.turaDvaOpt(1);
    auto finish3 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed3 = finish3 - start3;
    std::cout <<endl<< "Vrijeme izvrsavanja:  " << elapsed3.count() << " s\n"<<endl;

    cout<<"Tura najmanja grana + 2-opt"<<endl<<"Duzina ture: ";
    auto start4 = std::chrono::high_resolution_clock::now();
    cout<<G.turaDvaOpt(2);
    auto finish4 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed4 = finish4 - start4;
    std::cout <<endl<< "Vrijeme izvrsavanja:  " << elapsed4.count() << " s\n"<<endl;

    return 0;
}
