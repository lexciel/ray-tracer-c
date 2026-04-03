#include "arena.h"
#include "camera.h"
#include "colour.h"
#include "hittable.h"
#include "hittable_list.h"
#include "interval.h"
#include "material.h"
#include "sphere.h"
#include "utility.h"
#include "vec3.h"

int main() {

  arena world_mem = arena_init(1024 * 1024 * 32);
  /*lambertian *material_ground = make_lambertian(&world_mem, (colour){0.8, 0.8,
  0}); lambertian *material_center = make_lambertian(&world_mem, (colour){0.1,
  0.2, 0.5}); dialectric *material_left = make_dialectric(&world_mem, 1.50);
  dialectric *material_bubble = make_dialectric(&world_mem, 1.00/1.50);
  metal *material_right = make_metal(&world_mem, (colour){0.8, 0.6, 0.2}, 1.0);
  */

  /**
      lambertian material_left = make_lambertian((colour){0, 0, 1});
      lambertian material_right = make_lambertian((colour){1, 0, 0});
      scalar R = cos(pi/4);

      sphere left_sphere = (hittable*)make_sphere((point3){-R,0,-1}, R,
     (material*)&material_left); sphere right_sphere =
     (hittable*)make_sphere((point3){R,0,-1}, R, (material*)&material_right);
      */
  hittable_list *world = make_hitlist(&world_mem, 1024);
  /*
  hitlist_add(world, (hittable*) &left_sphere);
  hitlist_add(world, (hittable*) &right_sphere);*/

  /*
  sphere *main_sphere = (hittable*)make_sphere(&world_mem, (point3){0,0,-1.2},
  0.5, (material*)material_center); sphere *ground =
  (hittable*)make_sphere(&world_mem, (point3){0,-100.5,-1}, 100,
  (material*)material_ground); sphere *left_sphere =
  (hittable*)make_sphere(&world_mem, (point3){-1,0,-1}, 0.5,
  (material*)material_left); sphere *bubble_sphere =
  (hittable*)make_sphere(&world_mem, (point3){-1,0,-1}, 0.4,
  (material*)material_bubble); sphere *right_sphere =
  (hittable*)make_sphere(&world_mem, (point3){1,0,-1}, 0.5,
  (material*)material_right);

  hitlist_add(world, (hittable*) main_sphere);
  hitlist_add(world, (hittable*) ground);
  hitlist_add(world, (hittable*) left_sphere);
  hitlist_add(world, (hittable*) bubble_sphere);
  hitlist_add(world, (hittable*) right_sphere);
  */

  /*
  vec3 v = {0, 0, 2};
  vec3 w = {0, 2, 0};
  vec3_print(stderr, vec3_cross(v, w));
  */

  material *ground_mat =
      (material *)make_lambertian(&world_mem, (colour){0.5, 0.5, 0.5});
  hitlist_add(world,
              (hittable *)make_sphere(&world_mem, (point3){0, -1000, 0},
                                      (point3){0, -1000, 0}, 1000, ground_mat));
  pcg32_random_t rng;
  pcg32_seed(&rng, 42, 0);

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      double choose_mat = random_double(&rng);

      point3 center = {a + 0.9 * random_double(&rng), 0.2,
                       b + 0.9 * random_double(&rng)};

      // Check distance to ensure we don't overlap the big spheres
      if (vec3_length(vec3_sub(center, (point3){4, 0.2, 0})) > 0.9) {
        material *sphere_mat;

        if (choose_mat < 0.8) {
          // Diffuse (Lambertian)
          colour albedo = vec3_mult(vec3_random(&rng), vec3_random(&rng));
          sphere_mat = (material *)make_lambertian(&world_mem, albedo);
          point3 centre2 =
              vec3_add(center, (vec3){0, random_double_range(0, 0.5, &rng), 0});
          hitlist_add(world, (hittable *)make_sphere(&world_mem, center,
                                                     centre2, 0.2, sphere_mat));

        } else if (choose_mat < 0.95) {
          // Metal
          colour albedo = vec3_random_interval((interval){0.5, 1}, &rng);
          double fuzz = random_double_range(0, 0.5, &rng);
          sphere_mat = (material *)make_metal(&world_mem, albedo, fuzz);

          hitlist_add(world, (hittable *)make_sphere(&world_mem, center, center,
                                                     0.2, sphere_mat));

        } else {
          // Glass (Dielectric)
          sphere_mat = (material *)make_dialectric(&world_mem, 1.5);
          hitlist_add(world, (hittable *)make_sphere(&world_mem, center, center,
                                                     0.2, sphere_mat));
        }
      }
    }
  }

  material *mat1 = (material *)make_dialectric(&world_mem, 1.5);
  hitlist_add(world, (hittable *)make_sphere(&world_mem, (point3){0, 1, 0},
                                             (point3){0, 1, 0}, 1.0, mat1));

  material *mat2 =
      (material *)make_lambertian(&world_mem, (colour){0.4, 0.2, 0.1});
  hitlist_add(world, (hittable *)make_sphere(&world_mem, (point3){-4, 1, 0},
                                             (point3){-4, 1, 0}, 1.0, mat2));

  material *mat3 =
      (material *)make_metal(&world_mem, (colour){0.7, 0.6, 0.5}, 0.0);
  hitlist_add(world, (hittable *)make_sphere(&world_mem, (point3){4, 1, 0},
                                             (point3){4, 1, 0}, 1.0, mat3));

  scalar aspect_ratio = 16.0 / 9.0;
  int samples_per_pixel = 500;
  int image_width = 2000;
  int max_depth = 50;
  scalar vfov = 20;

  camera cam = default_cam();

  cam.aspect_ratio = aspect_ratio, cam.image_width = image_width,
  cam.samples_per_pixel = samples_per_pixel, cam.max_depth = max_depth,
  cam.vfov = vfov, cam.lookfrom = (point3){13, 2, 3};
  cam.lookat = (point3){0, 0, 0};
  cam.vup = (vec3){0, 1, 0};
  cam.defocus_angle = 0.6;
  cam.focus_dist = 10;
  int image_height = (int)(image_width / aspect_ratio);
  image_height = (image_height < 1) ? 1 : image_height;
  colour_buffer *buffer = arena_alloc(
      &world_mem,
      sizeof(colour_buffer) + sizeof(uint8_t) * image_height * image_width * 3);
  buffer->height = image_height;
  buffer->width = image_width;
  buffer->buffer = (pixel *)((uint8_t *)buffer + sizeof(colour_buffer));
  render(&cam, world, &rng, buffer);
  arena_free(&world_mem);
}
