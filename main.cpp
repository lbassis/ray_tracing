#include <iostream>
#include <fstream>
#include <vector>

#include "rtweekend.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

#include "omp.h"

struct ray_info {
  int i;
  int j;
  int samples;
  int width;
  int height;
  camera cam;
  hittable_list world;
  int max_depth;
};

color ray_color(const ray& r, const hittable& world, int depth);

color process_pixel(struct ray_info info) {

  color pixel_color(0, 0, 0);

  for (int s = 0; s < info.samples; s++) {
    auto u = (info.i + random_double()) / (info.width-1);
    auto v = (info.j + random_double()) / (info.height-1);

    ray r = info.cam.get_ray(u, v);
    pixel_color += ray_color(r, info.world, info.max_depth);
  }

  return correct_color(pixel_color, info.samples);;
}

void write_image(std::vector<color> matrix, int width, int height) {

  std::ofstream out("output.ppm");
  out << "P3\n" << width << " " << height << "\n255\n";

  std::vector<color>::iterator iter = matrix.begin();

  for (iter; iter < matrix.end(); iter++) {
    out << (*iter).x() << " " << (*iter).y() << " " << (*iter).z() << "\n";
  }

  out.close();
}

color ray_color(const ray& r, const hittable& world, int depth) {

  hit_record rec;

  if (depth <= 0) return color(0, 0, 0);

  /* using 0.001 to avoid shadow acne */
  if (world.hit(r, 0.001, infinity, rec)) {
    /* simple diffusion method */
    /* point3 target = rec.p + rec.normal + random_in_unit_sphere(); */

    /* true lambertian method */
    point3 target = rec.p + rec.normal + random_unit_vector();

    /* uniform scatter */
    /*point3 target = rec.p + random_in_hemisphere(rec.normal);*/

    return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth-1);
  }

  vec3 unit_direction = unit_vector(r.direction());
  auto t = 0.5*(unit_direction.y() + 1.0);

  return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

/* render the normal mapped to rgb */
color ray_normal_color(const ray& r, const hittable& world) {

  hit_record rec;

  if (world.hit(r, 0, infinity, rec)) {
    return 0.5 * color(rec.normal + color(1, 1, 1));
  }

  vec3 unit_direction = unit_vector(r.direction());
  auto t = 0.5*(unit_direction.y() + 1.0);

  return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main() {

  // image
  const auto aspect_ratio = 16.0/9.0;
  const int image_width = 200;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 100;
  const int max_depth = 50;
  auto matrix = std::vector<color>(image_width * image_height);

  // world
  hittable_list world;
  world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
  world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

  // camera
  camera cam;

  // render
  omp_set_num_threads(image_width * image_height);
#pragma omp parallel
  {
    int position = omp_get_thread_num();
    ray_info info = {static_cast<int>(position%image_width), image_height - static_cast<int>(position/image_width), samples_per_pixel, image_width, image_height, cam, world, max_depth};
    matrix[position] = process_pixel(info);
  }

  std::cerr << "\nDone\n";

  write_image(matrix, image_width, image_height);
}
