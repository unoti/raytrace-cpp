#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>

#include "ray.h"
#include "vec3.h"

// Usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385; // There's seriously no generally accepted C++ constant for pi?

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