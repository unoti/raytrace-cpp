#pragma once

#include "vec3.h"

// Adjust the pixel color for gamma correction, and scale it down for color averaging from multiple samples.
// pixel_color contains the sum of many different samples.
void color_correct(Color& pixel_color, int samples_per_pixel)
{
	// This divides the color by the number of samples, and gamma corrects for gamma = 2.
	// Note that gamma correction means raising the pixel to the power of 1/gamma.
	// So for gamma=2 that means square root.
	double scale = 1.0 / samples_per_pixel;
	for (int i = 0; i < 3; i++) {
		pixel_color.e[i] = sqrt(pixel_color.e[i] * scale);
	}
}