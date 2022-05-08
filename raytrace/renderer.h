#pragma once

#include "scene.h"
#include "outputmedia.h"


class Renderer
{
	public:
		Renderer(int t_samples_per_pixel, int t_max_depth = 100) : samples_per_pixel{ t_samples_per_pixel }, max_depth { t_max_depth } {}

		void render_frame(const Scene& scene, OutputSurface& surface);

	private:
		void color_correct(Color& pixel_color); // Blend multiple samples pixels and gamma correct.
		Color ray_color(const Ray& r, const WorldObject& world, int remaining_depth); // Cast a ray and determine the resulting color.

		int max_depth; // Number of times a ray can bounce. Limits recursion.
		int samples_per_pixel; // Number of rays we fire and average per media pixel.
};
