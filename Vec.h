#ifndef VEC_H
#define VEC_H

class Vec {
private:
	double x, y, z;
public:
	Vec();
	Vec(const double& xVal, const double& yVal, const double& zVal);

	double getX() const;
	double getY() const;
	double getZ() const;

	void setX(const double xVal);
	void setY(const double yVal);
	void setZ(const double zVal);

	void set(const double xVal, const double yVal, const double zVal);

	Vec operator+(const Vec& v) const;
	Vec operator-(const Vec& v) const;

	Vec operator*(const double& k) const;
	Vec operator/(const double& k) const;

	double module();

	Vec unit();

	void print();

};

#endif