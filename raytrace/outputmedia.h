#pragma once

#include <memory>
#include <string>
#include <fstream>
#include "vec3.h"

using namespace std;

// An OutputSurface is abstract class for a 2d output that receives one frame of rendering.
class OutputSurface
{
	public:
		OutputSurface(int t_width, int t_height) : width{ t_width }, height{ t_height } {}

		virtual void set_pixel(int x, int y, const Color c) = 0;

	public:
		int width;
		int height;
};

// An OutputMedia receives multiple frames out output over time during an animation.
class OutputMedia
{
	public:
		virtual shared_ptr<OutputSurface> get_frame(int frame) = 0;
};

// PpmOutputSurface : outputs a frame of rendering to a PPM image file.
// As currently written this class can only accept pixels in sequence from top-left to lower-right.
class PpmOutputSurface : public OutputSurface
{
	public:
		PpmOutputSurface(int t_width, int t_height, string t_filename) : OutputSurface{ t_width, t_height }, file{ t_filename, ios::out }
		{
			file << "P3\n" << width << ' ' << height << "\n255\n";
		}
			

		void set_pixel(int x, int y, const Color c) override;

	private:
		ofstream file;
};

