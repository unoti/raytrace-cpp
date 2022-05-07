# raytrace-cpp
Ray Tracing in C++

## Goals
* Still image rendering
* Refactor the code to be closer to what you'd like to see yourself
* 64 frame animated gif, looping
	* Use ideas you used to use regarding animated checkerboards, possibly the cubic spiral checkerboard with everysurface very reflective.
	* Possibly several spheres moving in an animated recurring spiral pattern, using radial coords and a sine function for the radius

## About
### Camera Geometry
This diagram describes the coordinate system, the camera strategy, the viewport,
and the meaning of the *u* and *v* variables in the main loop.
![Coordinates and Viewport](doc/img/fig-1.03-cam-geom.jpg)

## Running
```
release\raytrace > \tmp\x.ppm
```
Then go view the ppm file.

## Plan
1. [.] Go through all of *Ray Tracing in One Weekend*
1. [ ] Take a peek at the next 2 Ray Tracing books, in case they give you ideas so good you want to change this plan
1. [ ] Refactor it with easy to use abstractions and clean component decoupling, make it modular
1. [ ] Add BMP output, or possibly something more compact so you can include the latest render in the readme.
1. [ ] Animation. Select a good modern animated output format, and probably integrate a library for this
1. [ ] Add SDL
1. [ ] Add DearIMGUI

## Ideas
* Make it work with SDL
* Integrate IMGUI
* Abstract the underlying hardware interface (SDL) to make the same code work in a variety of circuumstances, including web, sdl, unreal
* Make the C++ code a Python library
* Try a version implemented in Python using Numpy

## Questions
These are questions I'd like to discuss with other C++ developers.
* **Lower case class/type names.** Does it make more sense to use a capital letter at the start of class names?  Using all lower case makes code seem harder to read or skim when trying to find the important things.
	* This [C++ Style Guide](https://lefticus.gitbooks.io/cpp-best-practices/content/03-Style.html) seems to agree with me.
	* [Google's C++ Style Guide](https://google.github.io/styleguide/cppguide.html#General_Naming_Rules) also suggests proper case for class names.
* **Style guides.** The above style guide suggests `m_` prefixes for member variables. I can get down with that, although I don't like the `t_` prefix they recommend.  I need to read more code and review some different industry leader style guides and figure out what style I want to use.  It seems like there is a great deal of variety in styles used, and I don't see the same level of consensus in C++ style practice as compared with other languages I'm familiar with.  The Google style guide suggests a *trailing* underscore for member variables.
	* How do we feel about writing code in header files, as was done in sphere.h?
	* Decide if you want to use same line or new line braces
	* Do we want `public:` and `private:` to be at zero indent?
	* I feel like the code is over-using `auto` and it inhibits readability in some places.

## Resources

* *[Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)*