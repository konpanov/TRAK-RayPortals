#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "primitives/cube.h"
#include "primitives/plane.h"
#include "portal.h"
#include "primitives/sphere.h"
#include "primitives/triangle.h"
#include "vec3.h"
#include <memory>

void simple_light() {
  hittable_list world;

  auto ground = make_shared<lambertian>(color(0.4, 0.2, 0.1));
  auto red = make_shared<lambertian>(color(1, 0, 0));
  auto pink = make_shared<lambertian>(color(0.8, 0.4, 0.4));
  auto mirror = make_shared<metal>(color(.9, 1, .9), 0);
  auto light = make_shared<diffuse_light>(color(4, 4, 4));
  // auto portal_mat = make_shared<portal_material>();
  // auto portal_out_mat = make_shared<portal_out>();

  world.add(make_shared<sphere>(point3(-5, -1000, -2), 1000, ground));
  // world.add(make_shared<sphere>(point3(0, 2, 0), 2, ground));
  // world.add(make_shared<sphere>(point3(3, 1, 2), 1, ground));
  // world.add(make_shared<sphere>(point3(3, 1, 4), 1, ground));

  // auto entry =
  //     triangle(point3(3, 1, 2), point3(5, 1, 2), point3(3, 3, 2),
  //     portal_mat);
  //
  // auto exit =
  //     triangle(point3(3, 1, 4), point3(5, 1, 4), point3(3, 3, 4),
  //     portal_mat);

  //
  //
  // auto entry = triangle(point3(3, 1, -1), point3(10, 1, -1), point3(3, 3,
  // -.5),
  //                       portal_mat);

  // auto exit =
  //     triangle(point3(-3, 3, 4), point3(10, 3, 4), point3(3, 3, 4),
  //     portal_mat);
  // world.add(make_shared<portal>(entry, exit));

  auto difflight = make_shared<diffuse_light>(color(8, 8, 8));
  world.add(make_shared<sphere>(point3(4, 2, -2), 1, difflight));

  world.add(
      make_shared<plane>(new point3[4]{point3(20, 0, 6), point3(-26, 0, -6),
                                       point3(26, 30, 6), point3(-26, 30, -6)},
                         pink));

  camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth = 50;
  cam.background = color(.1, .1, .1);

  cam.vfov = 20;
  cam.lookfrom = point3(26, 3, 6);
  cam.lookat = point3(0, 2, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0;

  cam.render(world);
}

void cornell_box() {
  hittable_list world;

  auto red = make_shared<lambertian>(color(.65, .05, .05));
  auto white = make_shared<lambertian>(color(.73, .73, .73));
  auto green = make_shared<lambertian>(color(.12, .45, .15));
  auto light = make_shared<diffuse_light>(color(15, 15, 15));

  world.add(make_shared<quad>(point3(555, 0, 0), vec3(0, 555, 0),
                              vec3(0, 0, 555), green));
  world.add(make_shared<quad>(point3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555),
                              red));

  // Above box, out of the camera view
  world.add(make_shared<triangle>(point3(0, 600, 0), vec3(130, 600, 0),
                              vec3(0, 600, 130), light)); //light
  world.add(make_shared<quad>(point3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555),
                              white));
  world.add(make_shared<quad>(point3(555, 555, 555), vec3(-555, 0, 0),
                              vec3(0, 0, -555), white));
  world.add(make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0),
                              vec3(0, 555, 0), white));

  // world.add(make_shared<sphere>(point3(343 - 65, 480, 332 - 52), 50, green));
  // world.add(make_shared<cube>(point3(400, 480, 400), vec3(0, -100, 0),
  //                             vec3(-100, 0, 0), vec3(-20, 0, -90), green));
  // world.add(make_shared<triangle>(vec3(363 - 150 * 2, 530, 352),
  //                                 point3(363, 530, 352),
  //                                 vec3(363, 530, 352 - 125 * 2), green));

  auto p_out = make_shared<portal_out>();
  //
  //
  // Under light, out of the camera view
  auto out_portal = make_shared<triangle>(point3(0, 580, 0),
                                          vec3(130, 580, 0),
                                          vec3(0, 580, 130), p_out);
  world.add(out_portal);
  auto p_in = make_shared<portal_in>(out_portal);

  // Inside the box, near the floor
  world.add(make_shared<triangle>(point3(270, 150, 270),
                                  vec3(270 - 130, 150, 270),
                                  vec3(270, 150, 270 - 130), p_in));

  // world.add(make_shared<triangle>(point3(363, 514, 352),
  //                                 vec3(363 - 150 * 2, 514, 352),
  //                                 vec3(363, 514, 332 - 125 * 2), green));

  auto p2_out = make_shared<portal_out>();
  auto out_portal2 = make_shared<triangle>(point3(270, 100, 270),
                                         vec3(270 - 130, 100, 270),
                                         vec3(270, 100, 270 - 130), p2_out);
  world.add(out_portal2);
  auto p2_in = make_shared<portal_in>(out_portal2);
  world.add(make_shared<triangle>(point3(50, 150, 270),
                                  vec3(50, 150 - 130, 270),
                                  vec3(50, 150, 270 + 130), p2_in));


  camera cam;

  cam.aspect_ratio = 1.0;
  cam.image_width = 720;
  cam.samples_per_pixel = 300;
  cam.max_depth = 100;
  cam.background = color(.1, .1, .1);

  cam.vfov = 40;
  cam.lookfrom = point3(278, 278, -1200);
  cam.lookat = point3(278, 278, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0;

  cam.render(world);
}

int main() {
  switch (2) {
  case 1:
    simple_light();
    break;
  case 2:
    cornell_box();
    break;
  }
}
