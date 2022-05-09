#pragma once

#include "worldobject.h"
#include "vec3.h"
#include "material.h"

class Sphere : public WorldObject {
	public:
		Sphere() : radius(1) {}
		Sphere(Point3 cen, double r, shared_ptr<Material> m) : center(cen), radius(r), mat_ptr(m) {};

		virtual bool hit(
			const Ray& r, double t_min, double t_max, hit_record& rec) const override;

	public:
		Point3 center;
		double radius;
		shared_ptr<Material> mat_ptr;
};
