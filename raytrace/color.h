#pragma once

#include "vec3.h"

#include <iostream>

//*TODO: this is specific to PPM format. Consider moving this to the surface class when you create it.
//*TODO: I don't like how this conflates averaging multiple pixel colors with the file format.
// This assumes that pixel_color contains the sum of multiple pixels which will be blended/averaged here.
void write_color(std::ostream& out, Color pixel_color, int samples_per_pixel)
{
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	// Divide the color by the number of samples, and gamma correct for gamma=2.
	auto scale = 1.0 / samples_per_pixel;
	r = sqrt(scale * r);
	g = sqrt(scale * g);
	b = sqrt(scale * b);

	// Write the translated [0,255] value of each color component.
	out << static_cast<int>(255.999 * clamp(r, 0, 0.999)) << ' ' // *TODO: refactor this into double_to_byte()?
		<< static_cast<int>(255.999 * clamp(g, 0, 0.999)) << ' ' // *TODO: might even just make a set of methods on vec3 that returns these as 0-255?
		<< static_cast<int>(255.999 * clamp(b, 0, 0.999)) << '\n';
}