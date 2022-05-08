#pragma once

#include "vec3.h"

/** A Ray is a straight line starting somewhere and proceeding in a direction to infinity.
	We can think of a ray as a function P(t) = A + tb
	A is the origin
	b is the ray direction
	t is a real number that moves the point along the ray.
	For positive t you get points in front of the ray.
*/
class Ray
{
	public:
		Ray() {}
		Ray(const Point3& origin, const Vec3& direction)
			: orig(origin), dir(direction) {} // this uses the compiler-generated default copy constructor?

		Point3 origin() const { return orig; }
		Vec3 direction() const { return dir; }

		Point3 at(double t) const {
			return orig + t * dir;
		}

	public:
		Point3 orig;
		Vec3 dir;
};