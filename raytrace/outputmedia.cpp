#include <memory>
#include <string>
#include <sstream>

#include "outputmedia.h"

using namespace std;

void PpmOutputSurface::set_pixel(int x, int y, const Color c)
{
	file << double_to_byte(c.x()) << ' '
		<< double_to_byte(c.y()) << ' '
		<< double_to_byte(c.z()) << '\n';
}

shared_ptr<OutputSurface> PpmOutputMedia::get_frame(int frame)
{
	stringstream s;
	s << filename_base;
	if (frame < 10)
		s << '0';
	s << frame << ".ppm";

	string filename = s.str();

	return make_shared<PpmOutputSurface>(width, height, filename);
}
