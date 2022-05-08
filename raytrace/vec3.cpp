#include "vec3.h"


Vec3 reflect(const Vec3& v, const Vec3& n) {
	return v - 2 * dot(v, n) * n;
}

// Refract a ray.
// refraction_ratio, or erai divided by etat.
Vec3 refract(const Vec3& uv, const Vec3& n, double refraction_ratio)
{
	auto cos_theta = fmin(dot(-uv, n), 1.0);
	Vec3 r_out_perp = refraction_ratio * (uv + cos_theta * n);
	Vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
	return r_out_perp + r_out_parallel;
}
