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
			auto r = double(i) / (image_width - 1);
			auto g = double(j) / (image_height - 1);
			auto b = 0.25;

			int ir = static_cast<int>(255.999 * r); // Don't like this code duplication here. double_to_byte()?
			int ig = static_cast<int>(255.999 * g);
			int ib = static_cast<int>(255.999 * b);

			std::cout << ir << ' ' << ig << ' ' << ib << '\n';
		}
	}
	std::cerr << "\nDone.\n";
}