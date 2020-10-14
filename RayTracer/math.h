#pragma once
#include <cmath>

struct vec
{
	double x, y, z;
	vec() : x(0), y(0), z(0) {}
	vec(double a, double b, double c) : x(a), y(b), z(c) {}
	vec(double a) : x(a), y(a), z(a) {}
	double length();
	vec normalize();
	vec reflect(const vec& n);
	void set(int i, double v) { switch (i) { case 0: x = v; case 1: y = v; case 2: z = v; } }
	vec operator - (const vec& v) const { return vec(x - v.x, y - v.y, z - v.z); }
	vec operator + (const vec& v) const { return vec(x + v.x, y + v.y, z + v.z); }
	vec operator / (double d) const { return vec(x / d, y / d, z / d); }
	vec operator * (const vec& v) const { return vec(x * v.x, y * v.y, z * v.z); }
	double operator [] (int i) const { switch (i) { case 0: return x; case 1: return y; case 2: return z; } };
};

vec operator * (double d, vec v);
vec operator * (vec v, double d);

double dot(const vec& a, const vec& b);
vec cross(const vec& a, const vec& b);

struct Ray
{
	vec o;		// origin of ray
	vec d;		// direction of ray
	double t;	// intersection point of ray
	Ray() : o(vec()), d(vec()), t(0) {}
	Ray(vec a, vec b, double c) : o(a), d(b), t(c) {}
	void set(double d) { t = d; }
};

vec random_in_unit_sphere();
