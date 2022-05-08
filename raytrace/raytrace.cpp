#include <iostream>

#include "raytrace.h"
#include "camera.h"
#include "color.h"
#include "world.h"
#include "sphere.h"
#include "material.h"
#include "scene.h"
#include "outputmedia.h"

// Returns a simple gradient for the color of a ray.
// The color is based on the Y value of where the ray points.
Color ray_color(const Ray& r, const WorldObject& world, int remaining_depth)
{
	const Color sky_color = Color(0.5, 0.7, 1.0); // Sky Blue
	//const Color sky_color = Color(1.0, 0.7, 0.5); // Dusk

	// If we've exceeded the bounce limit then no more light is gathered. Limits recursion depth.
	if (remaining_depth < 0)
		return Color(0, 0, 0);

	hit_record rec;
	// Some rays hit very close to zero due to float approximations, so use a small nonzero value for min.
	if (world.hit(r, 0.001, infinity, rec))
	{
		Ray scattered;
		Color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, remaining_depth - 1);
		return Color(0, 0, 0);
	}

	Vec3 unit_direction = r.direction().unit_vector();
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * sky_color;
}

Scene simple_scene(double aspect_ratio)
{
	auto R = cos(pi / 4);
	auto world = make_shared<World>();

	auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0));
	auto material_center = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
	auto material_left = make_shared<Dielectric>(1.5);
	auto material_right = make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0);

	world->add(make_shared<Sphere>(Point3(0, -100.5, -1), 100.0, material_ground));
	world->add(make_shared<Sphere>(Point3(0, 0.0, -1), 0.5, material_center));
	world->add(make_shared<Sphere>(Point3(-1, 0.0, -1), 0.5, material_left));
	world->add(make_shared<Sphere>(Point3(-1, 0.0, -1), -0.45, material_left)); // Negative radius: hollow bubble. Normal points inward.
	world->add(make_shared<Sphere>(Point3(1, 0.0, -1), 0.5, material_right));

	//const Point3 look_from = Point3(13, 2, 3);
	const Point3 look_from = Point3(2, 1, -5);
	const Point3 look_at = Point3(0,0,0);
	const Vec3 v_up = Vec3(0, 1, 0);
	const double field_of_view = 25;
	auto cam = make_shared<Camera>(look_from, look_at, v_up, field_of_view, aspect_ratio);

	return Scene(world, cam);
}

shared_ptr<Material> random_material()
{
	double d = random_double();
	if (d < 0.8)
	{
		// diffuse
		auto albedo = Color::random() * Color::random();
		return make_shared<Lambertian>(albedo);
	}
	else if (d < 0.95)
	{
		// metal
		auto albedo = Color::random(0.5, 1);
		auto fuzz = random_double(0, 0.5);
		return make_shared<Metal>(albedo, fuzz);
	}
	else
	{
		// glass
		return make_shared<Dielectric>(1.5);
	}
}

Scene random_scene(double aspect_ratio)
{
	auto world = make_shared<World>();

	auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
	world->add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			Point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
			if ((center - Point3(4, 0.2, 0)).length() < 0.9)
				continue;

			auto mat = random_material();
			world->add(make_shared<Sphere>(center, 0.2, mat));
		}
	}

	auto material1 = make_shared<Dielectric>(1.5);
	auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
	auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0);
	world->add(make_shared<Sphere>(Point3(0, 1, 0), 1, material1));
	world->add(make_shared<Sphere>(Point3(-4, 1, 0), 1, material2));
	world->add(make_shared<Sphere>(Point3(4, 1, 0), 1, material3));
	
	const Point3 look_from = Point3(13, 2, 3);
	const Point3 look_at = Point3(0, 0, 0);
	const Vec3 v_up = Vec3(0, 1, 0);
	const double field_of_view = 20;

	auto cam = make_shared<Camera>(look_from, look_at, v_up, field_of_view, aspect_ratio);
	return Scene(world, cam);
}


int main()
{
	// Image
	const double aspect_ratio = 16.0 / 9.0;
	const int image_width = 200;//400 // 1200;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 10;// 100; // 500;
	const int max_depth = 100; // Maximum number of light bounces.

	// Scene
	Scene scene = simple_scene(aspect_ratio);
	//Scene scene = random_scene(aspect_ratio);

	// Output
	//*TODO: do a better job with output file location handling. When you do, update the bat file as well.
	PpmOutputSurface surface = PpmOutputSurface(image_width, image_height, "/tmp/raytrace.ppm");

	// Render
	//std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; j--)	// y_pixel might be better
	{
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; i++) // x_pixel might be better
		{
			Color pixel_color(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; s++)
			{
				auto u = (i + random_double()) / (static_cast<double>(image_width) - 1);
				auto v = (j + random_double()) / (static_cast<double>(image_height) - 1);
				Ray r = scene.camera->get_ray(u, v);
				pixel_color += ray_color(r, *scene.world, max_depth);
			}
			color_correct(pixel_color, samples_per_pixel);
			surface.set_pixel(i, j, pixel_color);
		}
	}
	std::cerr << "\nDone.\n";
}