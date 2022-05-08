#pragma once

#include "raytrace.h"

class Material; // So we know below that it's a pointer to a class.

// Information about the intersection of a ray collision with something.
struct hit_record
{
	//*TODO: these names are a bit too terse. Code readability legitimately suffers at usage sites e.g. in sphere::hit().
	Point3 p; // Where the ray intersected in world space.
	Vec3 normal; // The normal of the object we intersected at the point of collision.
	shared_ptr<Material> mat_ptr;
	double t = 0.0; // How long the ray was at the point of intersection.
	bool front_face = false; // True if this hits the front or outside of the object.

	//*TODO: Not liking how this is a struct with a method that must/should be called on construction...
	// Determine if we're hitting the front vs back, or outside face vs inside face.
	inline void set_face_normal(const Ray& r, const Vec3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

// WorldObject is something that exists in the world, and is an tnterface for an object or volume that can collide with a ray.
// This corresponds to what Peter Shirley calls a "hittable".
// Unity calls this a Collider. I don't like the name "hittable"
// because it can erroneously be read as "Hit Table" which is *not* what this is.
class WorldObject
{
	public:
		virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};