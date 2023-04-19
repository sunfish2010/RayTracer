#ifndef COLOR_H
#define COLOR_H

#include <iostream>

#include "common/utils.hh"
#include "common/vec3.hh"

void write_color(std::ostream& out, Color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp<double>(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp<double>(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp<double>(b, 0.0, 0.999)) << '\n';
}

#endif