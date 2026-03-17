#include "camera.h"
#include "utility.h"
#include "vec3.h"
#include "material.h"
#include <math.h>

camera default_cam() {
    camera cam = {
    .aspect_ratio      = 1.0,  // Ratio of image width over height
    .image_width       = 100,  // Rendered image width in pixel count
    .samples_per_pixel = 10,   // Count of random samples for each pixel
    .max_depth         = 10,   // Maximum number of ray bounces into scene

    .vfov     = 90,              // Vertical view angle (field of view)
    .lookfrom = {0,0,0},   // Point camera is looking from
    .lookat   = {0,0,-1},  // Point camera is looking at
    .vup      = {0,1,0},     // Camera-relative "up" direction
    .defocus_angle = 0,
    .focus_dist = 10,
    } ;
    return cam;
}

void camera_init(camera* orig_cam) {
    camera temp_cam = *orig_cam;
    temp_cam.image_height = (int) (temp_cam.image_width/temp_cam.aspect_ratio);
    temp_cam.image_height = (temp_cam.image_height < 1) ? 1 : temp_cam.image_height;
    temp_cam.pixel_samples_scale = 1.0 / temp_cam.samples_per_pixel;

    temp_cam.centre = temp_cam.lookfrom;
    //scalar focal_len = vec3_length(vec3_sub(temp_cam.lookfrom, temp_cam.lookat));


    scalar theta = degrees_to_radians(temp_cam.vfov);
    scalar h = tan(theta/2);

    scalar viewport_height = 2.0 * h * temp_cam.focus_dist;
    scalar viewport_width = viewport_height * (((scalar)temp_cam.image_width)/temp_cam.image_height);

    temp_cam.w = vec3_unit(vec3_sub(temp_cam.lookfrom, temp_cam.lookat));
    temp_cam.u = vec3_unit(vec3_cross(temp_cam.vup, temp_cam.w));
    temp_cam.v = vec3_cross(temp_cam.w, temp_cam.u);

    vec3 viewport_x = vec3_scale(temp_cam.u, viewport_width);
    vec3 viewport_y = vec3_scale(temp_cam.v, -viewport_height);
    
    temp_cam.pixel_delta_x = vec3_scale(viewport_x, 1.0/temp_cam.image_width);
    temp_cam.pixel_delta_y = vec3_scale(viewport_y, 1.0/temp_cam.image_height);

    vec3 viewport_upper_left = vec3_sub(vec3_sub(temp_cam.centre,
                                        vec3_scale(temp_cam.w, temp_cam.focus_dist)),
                                        vec3_add(vec3_scale(viewport_x, 0.5),
                                        vec3_scale(viewport_y, 0.5)));
    temp_cam.pixel00_loc = vec3_add(viewport_upper_left, vec3_scale(vec3_add(temp_cam.pixel_delta_x, temp_cam.pixel_delta_y), 0.5));

    scalar defocus_radius = temp_cam.focus_dist * tan(degrees_to_radians(temp_cam.defocus_angle / 2));
    temp_cam.defocus_disk_x = vec3_scale(temp_cam.u, defocus_radius);
    temp_cam.defocus_disk_y= vec3_scale(temp_cam.v, defocus_radius);

    *orig_cam = temp_cam;
}

void render(camera *cam, hittable_list *world, pcg32_random_t *rng) {

    camera_init(cam);

    printf("P3\n%d %d\n255\n", cam->image_width, cam->image_height);

    for (int i =0; i<cam->image_height; i++) {
        fprintf(stderr, "\rScanlines remaining: %d", cam->image_height - i);
        vec3 row_start = vec3_add(cam->pixel00_loc, vec3_scale(cam->pixel_delta_y, i));
        for (int j = 0; j < cam->image_width; j++) {
            /*point3 pixel_centre = vec3_add(cam->pixel00_loc, vec3_add(vec3_scale(cam->pixel_delta_x, (scalar)j), vec3_scale(cam->pixel_delta_y, (scalar)i)));
            vec3 ray_direction = vec3_sub(pixel_centre, cam->centre);
            ray r = {cam->centre, ray_direction};
            colour pixel_colour = ray_colour(&r, (hittable*) world);*/
            colour pixel_colour = {0,0,0};
            vec3 pixel_centre = vec3_add(row_start, vec3_scale(cam->pixel_delta_x, j));
            for (int sample = 0; sample < cam->samples_per_pixel; sample++) {
                ray r = get_ray(cam, pixel_centre, rng);
                pixel_colour = vec3_add(pixel_colour, ray_colour(&r, (hittable*) world, cam->max_depth, rng));
            }
            colour_write(stdout, vec3_scale(pixel_colour,cam->pixel_samples_scale));
        }
    }
    
}

colour ray_colour(ray *r, const hittable *world, int depth, pcg32_random_t *rng) {

    if (!depth) return black;
            

    hit_record rec;

    if (world->hit(world, r, (interval){0.0001, infinity}, &rec)) {
        ray scattered;
        colour attentuation;
        if (rec.mat->scatter(rec.mat, r, &rec, &attentuation, &scattered, rng)) {
            return vec3_mult(ray_colour(&scattered, world, depth-1, rng), attentuation);
        }
        return black;
        /*vec3 direction = vec3_add(rec.normal, vec3_random_unit_vector());
        ray new_ray = {rec.p, direction};
        return vec3_scale(ray_colour(&new_ray, world, depth-1), 0.5);*/
    }
    vec3 unit_dir = vec3_unit(r->dir);
    scalar a = 0.5 * (unit_dir.y + 1);
    return vec3_add((colour){(1.0-a)*1, (1.0-a)*1, (1.0-a)*1}, (colour){0.5*a, 0.7*a, 1.0*a});

}

/*ray get_ray(const camera* cam, int i, int j) {
    vec3 offset = sample_square();
    vec3 pixel_sample = vec3_add(cam->pixel00_loc,
                        vec3_add(vec3_scale(cam->pixel_delta_x, (i + offset.x)),
                                    vec3_scale(cam->pixel_delta_y, (j + offset.y))));
    point3 ray_origin = cam->centre;
    vec3 ray_direction = vec3_sub(pixel_sample, ray_origin);
    return (ray){ray_origin, ray_direction};
}*/

vec3 sample_square(pcg32_random_t *rng) {
    return (vec3){random_double(rng) - 0.5, random_double(rng) - 0.5, 0};
}

point3 defocus_disk_sample(const camera *cam, pcg32_random_t *rng) {
    vec3 p = vec3_random_in_unit_disk(rng);
    return vec3_add(cam->centre, vec3_add(vec3_scale(cam->defocus_disk_x, p.x), vec3_scale(cam->defocus_disk_y, p.y)));
}