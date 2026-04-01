#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

typedef struct {
  hittable base;
  ray centre;
  scalar rad; // fmax(0, rad_input) when not using constructor
  material *mat;
} sphere;
extern const point3 static_sphere;
int hit_sphere(const hittable *self, const ray *r, interval ray_t,
               hit_record *rec);
sphere *make_sphere(arena *a, point3 centre1, point3 centre2, scalar r,
                    material *mat);
sphere *make_static_sphere(arena *a, point3 centre1, scalar r, material *mat);
#endif
