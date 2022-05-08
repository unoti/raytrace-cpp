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

bool Sphere::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const
{
	Vec3 oc = r.origin() - center; // Direction and length from origin to center of this sphere.
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius * radius;

	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0)
		return false;
	auto sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	auto root = (-half_b - sqrtd) / a;
	if (root < t_min || t_max < root) {
		root = (-half_b + sqrtd) / a;
		if (root < t_min || t_max < root)
			return false;
	}

	rec.t = root;
	rec.p = r.at(rec.t);
	Vec3 outward_normal = (rec.p - center) / radius;
	rec.set_face_normal(r, outward_normal);
	rec.mat_ptr = mat_ptr;

	return true;
}