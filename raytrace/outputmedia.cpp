
#include "outputmedia.h"

void PpmOutputSurface::set_pixel(int x, int y, const Color c)
{
	file << double_to_byte(c.x()) << ' '
		<< double_to_byte(c.y()) << ' '
		<< double_to_byte(c.z()) << '\n';
}


