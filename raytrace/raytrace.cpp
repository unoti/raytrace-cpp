#include <iostream>

#include "raytrace.h"
#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

// Returns a simple gradient for the color of a ray.
// The color is based on the Y value of where the ray points.
color ray_color(const ray& r, const hittable& world, int remaining_depth)
{
	const color sky_color = color(0.5, 0.7, 1.0); // Sky Blue
	//const color sky_color = color(1.0, 0.7, 0.5); // Dusk

	// If we've exceeded the bounce limit then no more light is gathered. Limits recursion depth.
	if (remaining_depth < 0)
		return color(0, 0, 0);

	hit_record rec;
	// Some rays hit very close to zero due to float approximations, so use a small nonzero value for min.
	if (world.hit(r, 0.001, infinity, rec))
	{
		//*TODO: Consider using the alternative diffuse formulation, random_in_hemisphere().
		point3 target = rec.p + rec.normal + vec3::random_in_unit_sphere();
		ray bounced_ray = ray(rec.p, target - rec.p);
		return 0.5 * ray_color(bounced_ray, world, remaining_depth-1);
	}

	vec3 unit_direction = r.direction().unit_vector();
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * sky_color;
}

int main()
{
	// Image
	const double aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 100;
	const int max_depth = 50; // Maximum number of light bounces.

	// World
	hittable_list world;
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

	// Camera
	//*TODO: Consider refactoring this to move the random sampling to inside the camera.
	camera cam;

	// Render
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; j--)	// y_pixel might be better
	{
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; i++) // x_pixel might be better
		{
			color pixel_color(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; s++)
			{
				auto u = (i + random_double()) / (image_width - 1);
				auto v = (j + random_double()) / (image_height - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world, max_depth);
			}
			write_color(std::cout, pixel_color, samples_per_pixel);
		}
	}
	std::cerr << "\nDone.\n";
}