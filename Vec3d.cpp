#include "Vec3d.h"
#include <math.h>
#include <iostream>
using namespace std;

//constructors
Vec::Vec() {
	x = y = z = 0;
}

Vec::Vec(const double& xIni, const double& yIni, const double& zIni) {
	x = xIni;
	y = yIni;
	z = zIni;
}

//set
void Vec::setX(const double xVal) { x = xVal; }
void Vec::setY(const double yVal) { y = yVal; }
void Vec::setZ(const double zVal) { z = zVal; }

void Vec::set(const double xVal, const double yVal, const double zVal) {
	x = xVal;
	y = yVal;
	z = zVal;
}

//get
double Vec::getX() const { return x; }
double Vec::getY() const { return y; }
double Vec::getZ() const { return z; }

//operators
Vec Vec::operator+(const Vec& v) const {
	return Vec(x + v.getX(), y + v.getY(), z + v.getZ());
}
Vec Vec::operator-(const Vec& v) const {
	return Vec(x - v.getX(), y - v.getY(), z - v.getZ());
}
Vec Vec::operator*(const double& k) const {
	return Vec(x * k, y * k, z * k);
}
Vec Vec::operator/(const double& k) const {
	return Vec(x / k, y / k, z / k);
}

//vec atributes
double Vec::module() { 
	return sqrt(x*x + y*y + z*z);
	}

Vec Vec::unit() {
	double m = sqrt(x*x + y*y + z*z);
	return Vec(x/m, y/m, z/m);
}

//print
void Vec::print() {
	cout << endl << "X: " << x << endl << "Y: " << y << endl << "Z: " << z << endl;
}

