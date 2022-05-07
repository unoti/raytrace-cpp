#include <iostream>
#include "color.h"
#include "ray.h"
#include "vec3.h"

// Determines if a ray intersects a sphere.
// Returns the length of the ray (that is, the value for t) at the position where we hit.
// The length of the ray will be a value from -1 to +1.
// If we do not hit the sphere it will return -1.
//*TODO: clean up this description.
double hit_sphere(const point3& center, double radius, const ray& r)
{
	vec3 oc = r.origin() - center;
	auto a = dot(r.direction(), r.direction());
	auto b = 2.0 * dot(oc, r.direction());
	auto c = dot(oc, oc) - radius * radius;
	auto discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		return -1.0;
	}
	else {
		return (-b - sqrt(discriminant)) / (2.0 * a);
	}
	return (discriminant > 0);
}

// Returns a simple gradient for the color of a ray.
// The color is based on the Y value of where the ray points.
color ray_color(const ray& r)
{
	double t = hit_sphere(point3(0, 0, -1), 0.5, r);
	if (t > 0) {
		vec3 normal = unit_vector(r.at(t) - vec3(0, 0, -1));
		// Map the normal components to rgb.
		return 0.5 * color(normal.x() + 1, normal.y() + 1, normal.z() + 1);
	}

	vec3 unit_direction = unit_vector(r.direction());
	t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
	// Image
	const double aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);

	// Camera
	const double viewport_height = 2.0;
	const double viewport_width = aspect_ratio * viewport_height;
	const double focal_length = 1.0;

	point3 origin = point3(0, 0, 0);
	vec3 horizontal = vec3(viewport_width, 0, 0); // Direction and size of viewport horizontally.
	vec3 vertical = vec3(0, viewport_height, 0); // Direction and size of viewport vertically.
	point3 lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

	// Render
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; j--)	// y_pixel might be better
	{
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; i++) // x_pixel might be better
		{
			auto u = double(i) / (image_width - 1); // u: 0-1 horizontal position across viewport.
			auto v = double(j) / (image_height - 1); // v: 0-1 vertical position across viewport.
			ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
			color pixel_color = ray_color(r);

			write_color(std::cout, pixel_color);
		}
	}
	std::cerr << "\nDone.\n";
}