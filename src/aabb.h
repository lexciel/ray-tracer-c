#ifndef AABB_H
#define AABB_H

#include "interval.h"
#include "ray.h"

typedef union {
  interval box[3];
  struct {
    interval x, y, z;
  };
} aabb;

extern const aabb aabb_empty;
extern const aabb aabb_universe;

void init_aabb(point3 a, point3 b, aabb *box);
int bb_hit(aabb *bb, const ray *r, interval ray_t);
aabb enclose_aabb(aabb box1, aabb box2);
int longest_axis(aabb box);
#endif
