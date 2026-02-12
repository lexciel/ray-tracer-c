#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"


typedef struct {
    hittable base;
    point3 centre;
    scalar rad; //fmax(0, rad_input) when not using constructor
    material *mat;
} sphere;

int hit_sphere(const hittable *self, const ray *r, interval ray_t, hit_record *rec) ;
sphere *make_sphere(arena *a, point3 c, scalar r, material *mat);
#endif
