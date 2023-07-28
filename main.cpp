#include <iostream>

#include "rtweekend.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

/* posso adicionar um sleep quando acerta a esfera pra mostrar a distribuiçao de carga */

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
  const int image_width = 400;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 100;
  const int max_depth = 50;

  // world
  hittable_list world;
  world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
  world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));
  
  // camera
  camera cam;
  
  // render
  std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

  for (int j = image_height - 1; j >= 0; j--) {
    std::cerr << "\rScanlines remaining: " << j << std::flush;
    for (int i = 0; i < image_width; i++) {

      color pixel_color(0, 0, 0);
      /* we will have 100 samples per pixel and use their average to calculate the color -> antialias*/
      for (int s = 0; s < samples_per_pixel; s++) {
	auto u = (i + random_double()) / (image_width-1);
	auto v = (j + random_double()) / (image_height-1);

	ray r = cam.get_ray(u, v);
	pixel_color += ray_color(r, world, max_depth);
      }      
      write_color(std::cout, pixel_color, samples_per_pixel);
    }
  }

  std::cerr << "\nDone\n";
}
