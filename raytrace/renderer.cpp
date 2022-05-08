
#include "renderer.h"
#include "raytrace.h"
#include "material.h"

void Renderer::color_correct(Color& pixel_color)
{
	// This divides the color by the number of samples, and gamma corrects for gamma = 2.
	// Note that gamma correction means raising the pixel to the power of 1/gamma.
	// So for gamma=2 that means square root.
	double scale = 1.0 / samples_per_pixel;
	for (int i = 0; i < 3; i++) {
		pixel_color.e[i] = sqrt(pixel_color.e[i] * scale);
	}
}

// Cast a ray and determine the resulting color.
Color Renderer::ray_color(const Ray& r, const WorldObject& world, int remaining_depth)
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


void Renderer::render_frame(const Scene& scene, OutputSurface& surface)
{
	double width_denom = static_cast<double>(surface.width) - 1;
	double height_denom = static_cast<double>(surface.height) - 1;

	for (int j = surface.height - 1; j >= 0; j--)	// y_pixel might be better
	{
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < surface.width; i++) // x_pixel might be better
		{
			Color pixel_color(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; s++)
			{
				auto u = (i + random_double()) / width_denom;
				auto v = (j + random_double()) / height_denom;
				Ray r = scene.camera->get_ray(u, v);
				pixel_color += ray_color(r, *scene.world, max_depth);
			}
			color_correct(pixel_color);
			surface.set_pixel(i, j, pixel_color);
		}
	}
}