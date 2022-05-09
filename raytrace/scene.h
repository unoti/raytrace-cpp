#pragma once

#include <vector>

#include "raytrace.h"
#include "world.h"
#include "camera.h"
#include "sphere.h"

// A Scene is a pairing of world objects plus camera parameters to look at them.
// It makes sense to bundle these together because if you've got some things
// in the world then you want the camera looking at them.  We typically switch the world
// objects and the camera at the same time, so they go together.
// When when animating a scene we often want the camera to animate and follow the action.
class Scene
{
public:
	Scene(shared_ptr<World> w, shared_ptr<Camera> c) : world{ w }, camera{ c } {}

	// For scenes that are animated, override this method and rearrange the world.
	// t is the time we are currently rendering and ranges from [0,1].
	virtual void set_time(double t) {}

public:
	shared_ptr<World> world;
	shared_ptr<Camera> camera;
};

class AnimatedScene1 : public Scene
{
	public:
		AnimatedScene1(double aspect_ratio) : Scene(make_shared<World>(), make_shared<Camera>(
			Point3(2, 1, -5), // look from
			Point3(0, 0, 0), // look at
			Point3(0, 1, 0), // vup
			40,//25, // field of view
			aspect_ratio))
		{
			auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0));
			auto material_center = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
			auto material_left = make_shared<Dielectric>(1.5);
			auto material_right = make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0);

			for (int i = 0; i < moving_sphere_count; i++) {
				shared_ptr<Sphere> sphere = make_shared<Sphere>(Point3(0, 0, 0), 0.5, material_right);
				spheres.push_back(sphere);
				world->add(sphere);
			}
			world->add(make_shared<Sphere>(Point3(0, -100.5, -1), 100.0, material_ground));
		}

		void set_time(double t) override
		{
			t /= static_cast<double>(moving_sphere_count); // The position of each sphere leads into the next one.
			double theta = 2 * pi * t;
			double a_offset = 2 * pi / moving_sphere_count;
			for (int i = 0; i < moving_sphere_count; i++)
			{
				double radius = 1;

				double a = theta + a_offset * i; // Base angle for this sphere.
				spheres[i]->center = Point3(
					radius * cos(a),
					0,
					radius * sin(a)
				);
			}
		}

	private:
		const int moving_sphere_count = 5;
		std::vector<shared_ptr<Sphere>> spheres;
};

