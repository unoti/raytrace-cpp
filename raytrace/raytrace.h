#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>

// Usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385; // There's seriously no generally accepted C++ constant for pi? Yes, not till C++20.

// Utility functions

inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

// Returns a random number in the range [0, 1).
inline double random_double() {
	return rand() / (RAND_MAX + 1.0);
}

// Returns a random number in the range [min, max).
inline double random_double(double min, double max) {
	return min + (max - min) * random_double();
}

inline double clamp(double x, double min, double max)
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

// Convert a double to a number from 0 to 255.
//*TODO: Investigate why the colors look wrong when I use uint8_t here.
inline int double_to_byte(double d)
{
	return static_cast<int>(255.999 * clamp(d, 0, 0.999));
}
