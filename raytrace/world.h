#pragma once

#include <memory>
#include <vector>

#include "worldobject.h"

using std::shared_ptr;

// The "World" is a collection of WorldObjects. It represents everything in the world.
class World : public WorldObject
{
	public:
		World() {}
		World(shared_ptr<WorldObject> object) { add(object); }

		void clear() { objects.clear(); }
		void add(shared_ptr<WorldObject> object) { objects.push_back(object); }

		virtual bool hit(
			const Ray& r, double t_min, double t_max, hit_record& rec) const override;

	public:
		std::vector<shared_ptr<WorldObject>> objects;
};

bool World::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const
{
	hit_record temp_rec;
	bool hit_anything = false;
	auto closest_so_far = t_max;

	for (const auto& object : objects) {
		if (object->hit(r, t_min, closest_so_far, temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}

	return hit_anything;
}