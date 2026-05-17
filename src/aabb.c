#include "aabb.h"
#include "interval.h"

const aabb aabb_empty = {
    .x = {.min = INFINITY, .max = -INFINITY},
    .y = {.min = INFINITY, .max = -INFINITY},
    .z = {.min = INFINITY, .max = -INFINITY},
};
const aabb aabb_universe = {
    .x = {.min = -INFINITY, .max = INFINITY},
    .y = {.min = -INFINITY, .max = INFINITY},
    .z = {.min = -INFINITY, .max = INFINITY},
};

void init_aabb(point3 a, point3 b, aabb *box) {
  box->x = (a.v[0] <= b.v[0]) ? (interval){a.v[0], b.v[0]}
                              : (interval){b.v[0], a.v[0]};
  box->y = (a.v[1] <= b.v[1]) ? (interval){a.v[1], b.v[1]}
                              : (interval){b.v[1], a.v[1]};
  box->z = (a.v[2] <= b.v[2]) ? (interval){a.v[2], b.v[2]}
                              : (interval){b.v[2], a.v[2]};
}

int bb_hit(aabb *bb, const ray *r, interval ray_t) {
  const point3 *ray_orig = &r->orig;
  const vec3 *ray_dir = &r->dir;

  for (int axis = 0; axis < 3; axis++) {
    const interval *ax = &bb->box[axis];
    const double adinv = 1.0 / ray_dir->v[axis];

    scalar t0 = (ax->min - ray_orig->v[axis]) * adinv;
    scalar t1 = (ax->max - ray_orig->v[axis]) * adinv;

    if (t0 < t1) {
      if (t0 > ray_t.min)
        ray_t.min = t0;
      if (t1 < ray_t.max)
        ray_t.max = t1;
    } else {
      if (t1 > ray_t.min)
        ray_t.min = t1;
      if (t0 < ray_t.max)
        ray_t.max = t0;
    }
    if (ray_t.max <= ray_t.min) {
      return 0;
    }
  }
  return 1;
}

aabb enclose_aabb(aabb box0, aabb box1) {
  return (aabb){.x = enclose_interval(box0.x, box1.x),
                .y = enclose_interval(box0.y, box1.y),
                .z = enclose_interval(box0.z, box1.z)};
}

int longest_axis(aabb box) {
  if (interval_size(box.x) > interval_size(box.y))
    return interval_size(box.x) > interval_size(box.z) ? 0 : 2;
  else
    return interval_size(box.y) > interval_size(box.z) ? 1 : 2;
}
