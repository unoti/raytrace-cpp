#pragma once

#include <cmath>
#include <limits>
#include <memory>

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