#pragma once
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include "math.h"


struct Shape
{
	bool isLight;
	vec mtl;
	Shape() : isLight(false) {  }
	virtual bool intersect(Ray& ray) const = 0;
	virtual vec normal(const vec& p) const = 0;
};

bool intersectRayWithPlane(const vec& p, const vec& n, Ray& ray);

struct Sphere : public Shape 
{
	const vec c;		// center of sphere
	double r;	// radius of sphere
	Sphere(const vec& a, double b) : c(a), r(b) { isLight = false; }
	Sphere(const vec& a, double b, bool c) : c(a), r(b) { isLight = c; }
	Sphere(const vec& a, double b, vec& m) : c(a), r(b) { mtl = m; }
	Sphere(const vec& a, double b, bool c, vec& m) : c(a), r(b) { mtl = m, isLight = c; }
	bool intersect(Ray& ray) const
	{
		//t = dot(oc, ray.d) +- sqrt( r^2 - oc^2 - dot(oc, ray.d)^2 )
		const vec oc = c - ray.o;
		const double b = dot(oc, ray.d);
		const double c = dot(oc, oc) - r * r;
		double h = b * b - c;
		if (h < 0.0) return false;
		h = sqrt(h);
		double t0 = b - h;
		double t1 = b + h;
		if (t0 < 0.0 && t1 < 0.0) return false;
		if (t0 < 0.0) { ray.t = t1; return false; }
		if (t1 < 0.0) { ray.t = t0; return false; }
		ray.set((t0 < t1) ? t0 : t1);
		return true;
	}
	vec normal(const vec& p) const
	{
		return (p - c).normalize();
	}
};

struct Triangle : public Shape
{
	vec v0, v1, v2;
	Triangle() : v0(vec()), v1(vec()), v2(vec()) { mtl = vec(); }
	Triangle(vec a, vec b, vec c) : v0(a), v1(b), v2(c) { mtl = vec(); }
	Triangle(vec a, vec b, vec c, vec m) : v0(a), v1(b), v2(c) { mtl = m; }
	bool intersect(Ray& ray) const
	{
		double oldT = ray.t;
		if (!intersectRayWithPlane(v0, normal(v0), ray)) { ray.set(oldT); return false; }
		vec ab = v1 - v0;
		vec ac = v2 - v0;
		vec p = ray.o + (ray.d * ray.t);
		vec ap = p - v0;

		double dotABAB = dot(ab, ab);
		double dotABAC = dot(ab, ac);
		double dotABAP = dot(ab, ap);
		double dotACAC = dot(ac, ac);
		double dotACAP = dot(ac, ap);

		double inv = 1 / (dotACAC * dotABAB - dotABAC * dotABAC);
		double u = (dotACAC * dotABAP - dotABAC * dotACAP) * inv;
		double v = (dotABAB * dotACAP - dotABAC * dotABAP) * inv;

		return u >= 0.0 && v >= 0.0 && u + v < 1.0;
	}
	vec normal(const vec& p) const
	{
		vec n = cross((v1-v0),(v2-v0)).normalize();
		return n;
	}
};


struct AABB : public Shape
{
	vec min, max;
	Shape* content;
	AABB(const vec& a, const vec& b) { min = a, max = b; content = nullptr; isLight = false; mtl = vec(); }
	AABB(const vec& a, const vec& b, bool c) { min = a, max = b; content = nullptr; isLight = c; mtl = vec(); }
	AABB(const vec& a, const vec& b, vec m) { min = a, max = b; content = nullptr; isLight = false; mtl = m; }
	AABB(const vec& a, const vec& b, bool c, vec m) { min = a, max = b; content = nullptr; isLight = c; mtl = m; }
	AABB(const vec& a, const vec& b, Shape* s) { min = a, max = b; content = s; isLight = false; mtl = vec(); }
	~AABB() { delete content; }
	bool intersect(Ray& ray) const
	{
		double tmin = std::numeric_limits<double>::min();
		double tmax = std::numeric_limits<double>::max();

		for (int i = 0; i < 3; i++)
		{
			float inv = 1.0 / ray.d[i];
			float t0 = (min[i] - ray.o[i]) * inv;
			float t1 = (max[i] - ray.o[i]) * inv;
			//std::cout << inv << " " << t0 << " " << t1 << std::endl;

			if (inv < 0.0) {
				float tmp = t0;
				t0 = t1;
				t1 = tmp;
			}
			
			tmin = t0 > tmin ? t0 : tmin;
			tmax = t1 < tmax ? t1 : tmax;

			//std::cout << tmin << " " << tmax << std::endl;

			if (tmax <= tmin) return false;
		}

		ray.set(tmin);

		return true;
	}
	vec normal(const vec& p) const
	{
		vec v(0,0,0);
		for (int i = 0; i < 3; i++)
		{
			if (abs(p[i] - min[i]) < 0.000001) {
				v.set(i, -1);
				return v;
			}
			if (abs(p[i] - max[i]) < 0.000001)
			{
				v.set(i, 1);
				return v;
			}
		}
		return vec();
	}
};

vec determineDiffuse(const Sphere& light, const vec& point, const vec& normal, const vec& col);
vec determineSpecular(const Sphere& light, const vec& point, const vec& normal, const vec& camera, double shiny, const vec& col);
