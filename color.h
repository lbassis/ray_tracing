#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

inline static double gamma_correct(double value, double gamma) {
  return gamma == 2.0 ? sqrt(value) : pow(value, 1/gamma);
}

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {

  auto r = pixel_color.x();
  auto g = pixel_color.y();
  auto b = pixel_color.z();

  // divide the color by the number of samples
  auto scale = 1.0 / samples_per_pixel;
  r = gamma_correct(scale * r, 2.0);
  g = gamma_correct(scale * g, 2.0);
  b = gamma_correct(scale * b, 2.0);
  
  out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << " "
      << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << " "
      << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << "\n";
}

color correct_color(color pixel_color, int samples_per_pixel) {

  auto r = pixel_color.x();
  auto g = pixel_color.y();
  auto b = pixel_color.z();

  // divide the color by the number of samples
  auto scale = 1.0 / samples_per_pixel;
  r = gamma_correct(scale * r, 2.0);
  g = gamma_correct(scale * g, 2.0);
  b = gamma_correct(scale * b, 2.0);

  return color(static_cast<int>(256 * clamp(r, 0.0, 0.999)),
	       static_cast<int>(256 * clamp(g, 0.0, 0.999)),
	       static_cast<int>(256 * clamp(b, 0.0, 0.999)));
}

#endif
