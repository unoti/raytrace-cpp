#pragma once

#include "raytrace.h"

class camera
{
	public:
		camera(
			Point3 look_from,
			Point3 look_at,
			Vec3 vup,
			double vertical_fov_degrees,
			double aspect_ratio
		) {
			auto theta = degrees_to_radians(vertical_fov_degrees);
			auto h = tan(theta / 2);		
			auto viewport_height = 2.0 * h;
			auto viewport_width = aspect_ratio * viewport_height;

			// u, v, w together form an orthonormal basis to describe the camera's
			// orientation along the look_from - look_at vector.
			auto w = (look_from - look_at).unit_vector();
			auto u = cross(vup, w).unit_vector();
			auto v = cross(w, u);

			origin = look_from;
			horizontal = viewport_width * u;
			vertical = viewport_height * v;
			lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
		}

		ray get_ray(double u, double v) const {
			return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
		}
	
	private:
		Point3 origin;
		Point3 lower_left_corner;
		Vec3 horizontal;
		Vec3 vertical;
};
