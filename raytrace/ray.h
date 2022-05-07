#pragma once

#include "vec3.h"

/**
	We can think of a ray as a function P(t) = A + tb
	A is the origin
	b is the ray direction
	t is a real number that moves the point along the ray.
	For positive t you get points in front of the ray.
*/
class ray
{
	public:
		ray() {}
		ray(const point3& origin, const vec3& direction)
			: orig(origin), dir(direction) {} // this uses the compiler-generated default copy constructor?

		point3 origin() const { return orig; }
		vec3 direction() const { return dir; }

		point3 at(double t) const {
			return orig + t * dir;
		}

	public:
		point3 orig;
		vec3 dir;
};