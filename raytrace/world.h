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
