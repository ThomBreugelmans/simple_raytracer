#include "world.h"

#include <vector>
#include "Color.h"
#include "libbmp.h"
#include "math.h"
#include "Shape.h"

bool World::addShape(Shape* s)
{
	if (s->isLight) {
		light = s; return true;
	}

	AABB* box = nullptr;
	if (Sphere* sp = dynamic_cast<Sphere*>(s))
	{
		vec min(sp->c.x - sp->r, sp->c.y - sp->r, sp->c.z - sp->r);
		vec max(sp->c.x + sp->r, sp->c.y + sp->r, sp->c.z + sp->r);
		box = new AABB(min, max, sp);
	}
	else if (AABB* b = dynamic_cast<AABB*>(s))
	{
		box = new AABB(b->min, b->max, b);
	}
	else if (Triangle* tr = dynamic_cast<Triangle*>(s))
	{
		vec min(std::numeric_limits<double>::max());
		vec max(std::numeric_limits<double>::min());
		for (int i = 0; i < 3; i++)
		{
			min.set(i, min[i] > tr->v0[i] ? tr->v0[i] : min[i]);
			min.set(i, min[i] > tr->v1[i] ? tr->v1[i] : min[i]);
			min.set(i, min[i] > tr->v2[i] ? tr->v2[i] : min[i]);

			max.set(i, max[i] < tr->v0[i] ? tr->v0[i] : max[i]);
			max.set(i, max[i] < tr->v1[i] ? tr->v1[i] : max[i]);
			max.set(i, max[i] < tr->v2[i] ? tr->v2[i] : max[i]);
		}
		box = new AABB(min, max, tr);
	}

	if (box == nullptr) return false;

	boundingBoxes.push_back(box);
	return true;
}

void World::castRays(bmp_img& img, const Color& White, const vec& bg, const vec& white, const double fov, const int W, const int H, const int hH, const int hW, const int sampleAmount)
{
	Color* col = &(White * bg);
	double f = hH / std::atan(fov);

	const int total_rays = W * H;
	int current_percentage = 1;
	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			col = &(White * bg);
			Ray ray(vec(0, 0, 0), vec(hW - x, hH - y, f).normalize(), std::numeric_limits<double>::max());


			std::vector<Shape*> shapes = checkBoundingBoxes(ray);
			if (!shapes.empty())
				col = &(White * color(*this, shapes, ray, sampleAmount, white, 0));


			col->clamp();
			bmp_pixel_init(&img.img_pixels[y][x], col->r, col->g, col->b);

			if (((x+1) * (y+1) * 100)/total_rays > current_percentage)
			{
				std::cout << "Currently at " << current_percentage << "%" << std::endl;
				current_percentage += 1;
			}
		}
	}
}

std::vector<Shape*> World::checkBoundingBoxes(Ray& ray) const
{
	std::vector<Shape*> shapes;
	double oldT = ray.t;
	for (auto s : boundingBoxes) {
		if (s->intersect(ray)) {
			ray.set(oldT);
			shapes.push_back(s->content);
		}
	}
	return shapes;
}

vec color(const World& world, const std::vector<Shape*>& shapes, Ray& ray, int sampleAmount, const vec& c, int depth)
{
	if (depth > 5) return vec(0, 0, 0);


	double t = ray.t;
	Shape* s = nullptr;
	for (auto shape : shapes) {
		if (shape->intersect(ray))
		{
			t = ray.t < t ? ray.t : t;
			s = shape;
		}
	}
	//if (s != nullptr) std::cout << t << std::endl;
	if ((*world.light).intersect(ray) && t > ray.t)
		return vec(1, 1, 1);	// return color of light

	// no intersection with light
	if (s == nullptr)
		//no intersection at all
		return vec(0.8, 0.8, 0.9);

	ray.set(t);
	const vec p = ray.o + (ray.d * t);
	const vec normal = s->normal(p);
	vec tmp(0, 0, 0);

	for (int i = 0; i < sampleAmount; i++) {
		const vec rd = (normal + random_in_unit_sphere()).normalize();
		Ray reflected(p, rd, std::numeric_limits<double>::max());

		// get all shapes with whichs boundingbox intersects with the ray
		const std::vector<Shape*> shps = world.checkBoundingBoxes(reflected);
		tmp = tmp + color(world, shps, reflected, sampleAmount, s->mtl, depth + 1);
	}
	tmp = tmp / sampleAmount;
	//if (!s->isLight)std::cout << s->mtl.x << " " << s->mtl.y << " " << s->mtl.z << std::endl;
	return s->mtl * (tmp);
}

void World::free()
{
	for (auto s : boundingBoxes)
		delete s;
}
