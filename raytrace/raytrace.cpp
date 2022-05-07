#include <iostream>
#include "color.h"
#include "vec3.h"


int main() {
	// Image
	const int image_width = 256;
	const int image_height = 256;

	// Render
	//*TODO: send to a file instead of stdout
	//*TODO: bitmap instead of PPM? Actually let's move the image surface into a class and abstract this.
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; j--)	// Why wouldn't we use y here?
	{
		// Would like a better progress indicator. Move this to a class?
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;

		for (int i = 0; i < image_width; i++) // Why wouldn't we use x here?
		{
			color pixel_color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0.25);
			write_color(std::cout, pixel_color);
		}
	}
	std::cerr << "\nDone.\n";
}