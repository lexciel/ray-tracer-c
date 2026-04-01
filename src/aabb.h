#ifndef AABB_H
#define AABB_H

#include "interval.h"
#include "ray.h"

typedef struct {
  interval x, y, z;
} aabb;

aabb *init_aabb(point3 a, point3 b, aabb *box) {
  box->x = (a.v[0] <= b.v[0]) ? (interval){a.v[0], b.v[0]}
                              : (interval){b.v[0], a.v[0]};
  box->y = (a.v[1] <= b.v[1]) ? (interval){a.v[1], b.v[1]}
                              : (interval){b.v[1], a.v[1]};
  box->z = (a.v[2] <= b.v[2]) ? (interval){a.v[2], b.v[2]}
                              : (interval){b.v[2], a.v[2]};
  return box;
}

#endif
