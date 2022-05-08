#pragma once

#include <cmath>
#include <iostream>

#include "raytrace.h"

using std::sqrt;

class Vec3
{
	public:
		Vec3() : e{ 0,0,0 } {}
		Vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

		double x() const { return e[0]; }
		double y() const { return e[1]; }
		double z() const { return e[2]; }

		Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
		double operator[] (int i) const { return e[i]; }
		double& operator[](int i) { return e[i]; }

		Vec3& operator+=(const Vec3& v)
		{
			e[0] += v.e[0];
			e[1] += v.e[1];
			e[2] += v.e[2];
			return *this;
		}

		Vec3& operator*=(const double t)
		{
			e[0] *= t;
			e[1] *= t;
			e[2] *= t;
		}

		Vec3& operator/=(const double t)
		{
			return *this *= 1 / t;
		}

		double length() const
		{
			return sqrt(length_squared());
		}

		double length_squared() const
		{
			return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
		}

		inline Vec3 unit_vector() const;

		// Return true if the vector is close to zero in all dimensions.
		bool near_zero() const {
			const double s = 1e-8;
			return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
		}

		inline static Vec3 random()
		{
			return Vec3(random_double(), random_double(), random_double());
		}

		inline static Vec3 random(double min, double max)
		{
			return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
		}

		// Returns a random unit vector that is somewhere within the unit sphere.
		static Vec3 random_in_unit_sphere()
		{
			while (true)
			{
				Vec3 v = Vec3::random(-1, 1);
				if (v.length_squared() >= 1)
					continue;
				return v;
			}
		}

		// True Lambertian - returns a random unit vector with a smooth distribution.
		static Vec3 random_unit_vector()
		{
			return Vec3::random_in_unit_sphere().unit_vector();
		}

	public:
		double e[3];
};

// Type aliases for vec3.
// Maybe later we'll want to use more typesafe aliases for colors and points.
// But for now, we're going for less code is better.
using Point3 = Vec3; // 3d point
using Color = Vec3; // rgb color.

// Utility functions

inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vec3 operator+(const Vec3& u, const Vec3& v)
{
	return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v)
{
	return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v)
{
	return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vec3 operator*(const Vec3& u, double t)
{
	return Vec3(u.e[0] * t, u.e[1] * t, u.e[2] * t);
}

inline Vec3 operator*(double t, const Vec3& v)
{
	return v * t;
}

inline Vec3 operator/(Vec3 v, double t)
{
	return (1 / t) * v;
}

//*TODO: Would I like it better if dot were a method or static class function instead of a global function?
inline double dot(const Vec3& u, const Vec3& v)
{
	return u.e[0] * v.e[0]
		 + u.e[1] * v.e[1]
		 + u.e[2] * v.e[2];
}

inline Vec3 cross(const Vec3& u, const Vec3& v)
{
	return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
				u.e[2] * v.e[0] - u.e[0] * v.e[2],
				u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vec3 Vec3::unit_vector() const
{
	return *this / length();
}


Vec3 reflect(const Vec3& v, const Vec3& n);

// Refract a ray.
// refraction_ratio, or erai divided by etat.
Vec3 refract(const Vec3& uv, const Vec3& n, double refraction_ratio);
