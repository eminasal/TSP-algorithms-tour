#ifndef GRAD_H
#define GRAD_H

struct Grad {
  int indeks;
  double latitude;
  double longitude;
  Grad(int indeks, double latitude, double longitude):indeks(indeks), latitude(latitude), longitude(longitude) {}
};
#endif // GRAD_H
