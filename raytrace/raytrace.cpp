#include <iostream>

#include "raytrace.h"
#include "camera.h"
#include "world.h"
#include "sphere.h"
#include "material.h"
#include "scene.h"
#include "outputmedia.h"
#include "renderer.h"

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
	const int image_width = 200; //400 // 1200;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 10;// 100; // 500;
	const int max_depth = 100; // Maximum number of light bounces.

	// Scene
	Scene scene = simple_scene(aspect_ratio);
	//Scene scene = random_scene(aspect_ratio);

	// Output
	//*TODO: do a better job with output file location handling. When you do, update the bat file as well.
	PpmOutputMedia output_media = PpmOutputMedia(image_width, image_height, "/tmp/raytrace");
	auto surface = output_media.get_frame(0);

	// Render
	Renderer renderer = Renderer(samples_per_pixel, max_depth);
	renderer.render_frame(scene, *surface);

	std::cerr << "\nDone.\n";
}