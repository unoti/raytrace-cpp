#pragma once

#include "vec3.h"

#include <iostream>

void write_color(std::ostream& out, color pixel_color) {
	// Write the translated [0,255] value of each color component.
	out << static_cast<int>(255.999 * pixel_color.x()) << ' ' // *TODO: here's that duplicated double_to_int() function again. DRY this
		<< static_cast<int>(255.999 * pixel_color.y()) << ' ' // *TODO: might even just make a set of methods on vec3 that returns these as 0-255?
		<< static_cast<int>(255.999 * pixel_color.z()) << '\n';
}