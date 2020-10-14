#include <cmath>
#include <random>
#include "math.h"

double dot(const vec& a, const vec& b) {
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

vec cross(const vec& a, const vec& b)
{
	double x = a[1] * b[2] - a[2] * b[1];
	double y = a[2] * b[0] - a[0] * b[2];
	double z = a[0] * b[1] - a[1] * b[0];
	return vec(x, y, z);
}

vec operator * (double d, vec v) { return vec(v.x * d, v.y * d, v.z * d); }
vec operator * (vec v, double d) { return vec(v.x * d, v.y * d, v.z * d); }

double vec::length() { return sqrt(dot(*this, *this)); }
vec vec::normalize() { return ((*this) / this->length()); }
vec vec::reflect(const vec& n) 
{
	vec r(-x,-y,-z);
	vec tmp = n * dot(r, n) * 2;
	r = r - tmp;
	return r;
}

vec random_in_unit_sphere() {
	std::random_device rd;
	std::mt19937 e2(rd());
	std::uniform_real_distribution<> dist(-1, 1);

	vec p = vec(dist(e2), dist(e2), dist(e2)).normalize();
	return p;
}

