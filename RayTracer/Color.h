#pragma once
#include "math.h"

struct  Color
{
	int r, g, b;
	Color() : r(0),g(0),b(0) {};
	Color(int a, int b, int c) : r(a), g(b), b(c) {}
	Color operator * (double d) const { return Color(r*d,g*d,b*d); }
	Color operator * (vec d) const { return Color(r * d.x, g * d.y, b * d.z); }
	void clamp() { if (r > 255) r = 255; if (g > 255) g = 255; if (b > 255) b = 255; };
};
