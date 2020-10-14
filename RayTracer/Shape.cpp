#include "Shape.h"
#include <cmath>
#include "math.h"

bool intersectRayWithPlane(const vec& p, const vec& n, Ray& ray)
{
	double num = dot(n, p) - dot(n, ray.o);
	double denom = dot(n, ray.d);
	if (abs(denom) < 0.000001) return false;

	double t = num / denom;
	if (t < 0.0) return false;
	ray.set(t);
	return true;
}

vec determineDiffuse(const Sphere& light, const vec& point, const vec& normal, const vec& col)
{
	//std::cout << point.x << " " << point.y << " " << point.z << std::endl;
	vec pl = (light.c - point);
	double d = dot(normal, pl.normalize());
	if (d < 0.0)
		return vec();

	vec newCol = col * d;

	return newCol;
}
vec determineSpecular(const Sphere& light, const vec& point, const vec& normal, const vec& camera, double shiny, const vec& col)
{
	const vec pl = light.c - point;
	const vec pc = camera - point;
	const vec h = (pl + pc).normalize();
	double d = dot(h, normal);
	if (d < 0.0) return vec();
	d = pow(d, shiny);
	return col * d;
}