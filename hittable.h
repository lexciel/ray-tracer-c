#ifndef HITTABLE_H
#define HITTABLE_H

#include "utility.h"
#include "interval.h"
#include "ray.h"

typedef struct material material;
struct hittable;
typedef struct {
    point3 p;
    vec3 normal;
    material *mat;
    scalar t;
    int front_face;
} hit_record;


typedef int (*hit_f)(const struct hittable *self, const ray *r, interval ray_t, hit_record *rec) ;

typedef struct hittable {
    hit_f hit;
} hittable;

void set_face_normal(hit_record *self, const ray *r, const vec3 *outward_normal);
#endif