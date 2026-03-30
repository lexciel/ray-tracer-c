#ifndef RAY_H
#define RAY_H

#include "vec3.h"

typedef struct {
  point3 orig;
  vec3 dir;
  scalar tm;
} ray;

static inline point3 ray_at(scalar t, const ray *r) {
  return vec3_add(r->orig, vec3_scale(r->dir, t));
}

static inline scalar ray_time(const ray *r) { return r->tm; }

#endif
