#include <iostream>

#include "raytrace.h"
#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"

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
		ray scattered;
		color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, remaining_depth - 1);
		return color(0, 0, 0);
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
	const int max_depth = 100; // Maximum number of light bounces.

	// World
	auto R = cos(pi / 4);
	hittable_list world;

	auto material_left = make_shared<lambertian>(color(0, 0, 1));
	auto material_right = make_shared<lambertian>(color(1, 0, 0));

	world.add(make_shared<sphere>(point3(-R, 0, -1), R, material_left));
	world.add(make_shared<sphere>(point3( R, 0, -1), R, material_right));
	/*
	auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0));
	auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
	auto material_left = make_shared<dielectric>(1.5);
	auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

	world.add(make_shared<sphere>(point3( 0, -100.5, -1), 100.0, material_ground));
	world.add(make_shared<sphere>(point3( 0,    0.0, -1),   0.5, material_center));
	world.add(make_shared<sphere>(point3(-1,    0.0, -1),   0.5, material_left));
	world.add(make_shared<sphere>(point3(-1,    0.0, -1),  -0.4, material_left)); // Negative radius: hollow bubble. Normal points inward.
	world.add(make_shared<sphere>(point3( 1,    0.0, -1),   0.5, material_right));
	*/

	// Camera
	//*TODO: Consider refactoring this to move the random sampling to inside the camera.
	camera cam(90.0, aspect_ratio);

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
				auto u = (i + random_double()) / (static_cast<double>(image_width) - 1);
				auto v = (j + random_double()) / (static_cast<double>(image_height) - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world, max_depth);
			}
			write_color(std::cout, pixel_color, samples_per_pixel);
		}
	}
	std::cerr << "\nDone.\n";
}