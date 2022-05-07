#pragma once

#include "raytrace.h"
#include "hittable.h"
//struct hit_record;

// A material produces a scattered ray, and if scattered, says how much the ray should be attenuated.
// The material will tell us how rays interact with the surface of an object.
class material
{
	public:
		virtual bool scatter(
			const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
		) const = 0;
};

// A diffuse material.
class lambertian : public material
{
	public:
		lambertian(const color& a) : albedo(a) {}

		virtual bool scatter(
			const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
		) const override
		{
			auto scatter_direction = rec.normal + vec3::random_unit_vector();

			// Catch degenerate scatter direction -- happens if scatter direction is opposite the normal.
			// Not catching this leads to NaNs and inifnities, which can be bed. Stop it at the source here.
			if (scatter_direction.near_zero())
				scatter_direction = rec.normal;

			scattered = ray(rec.p, scatter_direction);
			attenuation = albedo;
			return true;
		}
	public:
		color albedo;
};

class metal : public material
{
	public:
		metal(const color& a) : albedo(a) {}

		virtual bool scatter(
			const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
		) const override
		{
			vec3 reflected = reflect(r_in.direction().unit_vector(), rec.normal);
			scattered = ray(rec.p, reflected);
			attenuation = albedo;
			return (dot(scattered.direction(), rec.normal) > 0);
		}
	public:
		color albedo;
};
