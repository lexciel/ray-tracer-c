#ifndef CAMERA_H
#define CAMERA_H

#include "hittable_list.h"
#include "colour.h"
#include "utility.h"
typedef struct {
    scalar aspect_ratio;
    int image_width;
    int samples_per_pixel;
    int image_height;   // Rendered image height
    int max_depth;
    scalar pixel_samples_scale; 

    scalar vfov;

    point3 centre;         // Camera centre
    point3 pixel00_loc;    // Location of pixel 0, 0
    vec3   pixel_delta_x;  // Offset to pixel to the right
    vec3   pixel_delta_y;  // Offset to pixel below

    point3 lookfrom;
    point3 lookat;
    vec3 vup;

    vec3 u, v, w;

    scalar defocus_angle;
    scalar focus_dist;

    vec3 defocus_disk_x;
    vec3 defocus_disk_y;
} camera;

point3 defocus_disk_sample(const camera *cam, pcg32_random_t *rng);
camera default_cam();
void camera_init(camera* orig_cam);
void render(camera *cam, hittable_list *world, pcg32_random_t *rng, colour_buffer *buffer);
colour ray_colour(ray *r, const hittable *world, int depth, pcg32_random_t *rng);
vec3 sample_square(pcg32_random_t *rng);

static inline ray get_ray(const camera *cam, point3 pixel_center, pcg32_random_t *rng) {
    // 1. Calculate random jitter (-0.5 to +0.5)
    double offset_x = random_double(rng) - 0.5;
    double offset_y = random_double(rng) - 0.5;

    // 2. Nudge the center point
    // We only multiply the offsets (small numbers), not the whole grid coordinates.
    point3 pixel_sample = {
        .x = pixel_center.x + (offset_x * cam->pixel_delta_x.x) + (offset_y * cam->pixel_delta_y.x),
        .y = pixel_center.y + (offset_x * cam->pixel_delta_x.y) + (offset_y * cam->pixel_delta_y.y),
        .z = pixel_center.z + (offset_x * cam->pixel_delta_x.z) + (offset_y * cam->pixel_delta_y.z)
    };

    point3 ray_origin = (cam->defocus_angle <= 0) ? cam->centre : defocus_disk_sample(cam, rng);
    
    // 3. Direction = Sample - Origin
    vec3 ray_dir = {
        .x = pixel_sample.x - ray_origin.x,
        .y = pixel_sample.y - ray_origin.y,
        .z = pixel_sample.z - ray_origin.z
    };

    return (ray){ray_origin, ray_dir};
}


#endif