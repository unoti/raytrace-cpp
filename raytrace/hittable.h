#pragma once

#include "raytrace.h"

class material; // So we know below that it's a pointer to a class.

// Information about the intersection of a ray collision with something.
struct hit_record
{
	//*TODO: these names are a bit too terse. Code readability legitimately suffers at usage sites e.g. in sphere::hit().
	point3 p; // Where the ray intersected in world space.
	vec3 normal; // The normal of the object we intersected at the point of collision.
	shared_ptr<material> mat_ptr;
	double t; // How long the ray was at the point of intersection.
	bool front_face = false; // True if this hits the front or outside of the object.

	//*TODO: Not liking how this is a struct with a method that must/should be called on construction...
	// Determine if we're hitting the front vs back, or outside face vs inside face.
	inline void set_face_normal(const ray& r, const vec3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

// Interface for an object or volume that can collide with a ray.
// Unity calls this a Collider. What should I call it?
// I don't like the name "hittable"
// because it can erroneously be read as "Hit Table" which is *not* what this is.
class hittable
{
	public:
		virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};