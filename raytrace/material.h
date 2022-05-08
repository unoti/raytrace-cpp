#pragma once

#include "raytrace.h"
#include "worldobject.h"
//struct hit_record;

// A material produces a scattered ray, and if scattered, says how much the ray should be attenuated.
// The material will tell us how rays interact with the surface of an object.
class Material
{
	public:
		virtual bool scatter(
			const ray& r_in, const hit_record& rec, Color& attenuation, ray& scattered
		) const = 0;
};

// A diffuse material.
class Lambertian : public Material
{
	public:
		Lambertian(const Color& a) : albedo(a) {}

		virtual bool scatter(
			const ray& r_in, const hit_record& rec, Color& attenuation, ray& scattered
		) const override
		{
			auto scatter_direction = rec.normal + Vec3::random_unit_vector();

			// Catch degenerate scatter direction -- happens if scatter direction is opposite the normal.
			// Not catching this leads to NaNs and inifnities, which can be bed. Stop it at the source here.
			if (scatter_direction.near_zero())
				scatter_direction = rec.normal;

			scattered = ray(rec.p, scatter_direction);
			attenuation = albedo;
			return true;
		}
	public:
		Color albedo;
};

class Metal : public Material
{
	public:
		Metal(const Color& a, double fuzziness) : albedo(a), fuzz(fuzziness < 1 ? fuzziness : 1){}

		virtual bool scatter(
			const ray& r_in, const hit_record& rec, Color& attenuation, ray& scattered
		) const override
		{
			Vec3 reflected = reflect(r_in.direction().unit_vector(), rec.normal);
			scattered = ray(rec.p, reflected + fuzz * Vec3::random_in_unit_sphere());
			attenuation = albedo;
			return (dot(scattered.direction(), rec.normal) > 0);
		}
	public:
		Color albedo;
		double fuzz;
};

// A transparent material such as glass.
class Dielectric : public Material
{
	public:
		Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

		virtual bool scatter(
			const ray& r_in, const hit_record& rec, Color& attenuation, ray& scattered
		) const
		{
			attenuation = Color(1.0, 1.0, 1.0); // Should this be a parameter/class property, like albedo?
			double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

			Vec3 unit_direction = r_in.direction().unit_vector();
			double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
			double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

			// At certain angles light can't refract, it ends up bouncing inside forever.
			// This is why the water/air boundary looks 100% reflective from certain angles.
			Vec3 direction;
			bool cannot_refract = refraction_ratio * sin_theta > 1.0;
			if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
				direction = reflect(unit_direction, rec.normal);
			else
				direction = refract(unit_direction, rec.normal, refraction_ratio);

			scattered = ray(rec.p, direction);
			return true;
		}

	public:
		double ir; // Index of refraction

	private:
		static double reflectance(double cosine, double ref_idx)
		{
			// Glass reflectivity varies with angle. At steep angles it becomes a mirror.
			// This is Schlick's approximation for that.
			auto r0 = (1 - ref_idx) / (1 + ref_idx);
			r0 = r0 * r0;
			return r0 + (1 - r0) * pow((1 - cosine), 5);
		}
};
