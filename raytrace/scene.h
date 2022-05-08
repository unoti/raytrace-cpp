#pragma once

#include "world.h"
#include "camera.h"

// A Scene is a pairing of world objects plus camera parameters to look at them.
// It makes sense to bundle these together because if you've got some things
// in the world then you want the camera looking at them.  We typically switch the world
// objects and the camera at the same time, so they go together.
// When when animating a scene we often want the camera to animate and follow the action.
class Scene
{
public:
	Scene(shared_ptr<World> w, shared_ptr<Camera> c) : world{ w }, camera{ c } {}

public:
	shared_ptr<World> world;
	shared_ptr<Camera> camera;
};