#pragma once

#include "ray.h"

struct hit_record;

// WorldObject is something that exists in the world, and is an tnterface for an object or volume that can collide with a ray.
// This corresponds to what Peter Shirley calls a "hittable".
// Unity calls this a Collider. I don't like the name "hittable"
// because it can erroneously be read as "Hit Table" which is *not* what this is.
class WorldObject
{
	public:
		virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};