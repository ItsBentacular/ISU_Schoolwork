#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

using color = vec3;

void write_color(std::ostream& out, const color& pixel_color) {
    // rgb is same as x,y,z
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // translates the vector values [0,1] into a rgb range (0,255)
    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    //writes the colors in order to create a ppm image.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}


#endif