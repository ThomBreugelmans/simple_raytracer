#pragma once
#include <vector>
#include "Color.h"
#include "libbmp.h"
#include "math.h"
#include "Shape.h"


struct World
{
	std::vector<AABB*> boundingBoxes;
	Shape* light;

	World() : light(nullptr) {};
	bool addShape(Shape* s);

	void castRays(bmp_img& img, const Color& White, const vec& bg, const vec& white, const double fov, const int W, const int H, const int hH, const int hW, const int sampleAmount);

	std::vector<Shape*> checkBoundingBoxes(Ray& ray) const;
	void free();
};

vec color(const World& world, const std::vector<Shape*>& shapes, Ray& ray, int sampleAmount, const vec& c, int depth);
