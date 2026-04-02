#include "sphere.h"
#include "aabb.h"
#include "hittable.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"
#include <math.h>

const point3 static_sphere = {0, 0, 0};

int hit_sphere(const hittable *self, const ray *r, interval ray_t,
               hit_record *rec) {
  const sphere *s = (const sphere *)self;
  point3 curr_centre = ray_at(ray_time(r), &(s->centre));
  vec3 oc = vec3_sub(curr_centre, r->orig);
  scalar a = vec3_length_squared(r->dir);
  scalar h = vec3_dot(r->dir, oc);
  scalar c = vec3_length_squared(oc) - s->rad * s->rad;
  scalar disc = h * h - a * c;

  if (disc < 0) {
    return 0;
  }
  scalar sqrtd = sqrt(disc);

  scalar root = (h - sqrtd) / a;
  if (!surrounds(ray_t, root)) {
    root = (h + sqrtd) / a;
    if (!surrounds(ray_t, root)) {
      return 0;
    }
  }

  rec->t = root;
  rec->p = ray_at(rec->t, r);

  vec3 outward_normal = vec3_scale(vec3_sub(rec->p, curr_centre), 1.0 / s->rad);
  set_face_normal(rec, r, &outward_normal);
  rec->mat = s->mat;

  return 1;
}

aabb bounding_box_sphere(hittable *self) { return ((sphere *)self)->bbox; }

sphere *make_sphere(arena *a, point3 c1, point3 c2, scalar r, material *mat) {
  sphere *s = arena_alloc(a, sizeof(sphere));
  s->centre = (ray){c1, vec3_sub(c2, c1)};
  s->rad = fmax(0, r);
  s->base.hit = hit_sphere;
  s->base.bb = bounding_box_sphere;
  s->mat = mat;

  vec3 rvec = (vec3){s->rad, s->rad, s->rad};
  aabb box1, box2;
  init_aabb(vec3_sub(ray_at(0, &(s->centre)), rvec),
            vec3_add(ray_at(0, &(s->centre)), rvec), &box1);
  init_aabb(vec3_sub(ray_at(1, &(s->centre)), rvec),
            vec3_add(ray_at(1, &(s->centre)), rvec), &box2);
  s->bbox = enclose_aabb(&box1, &box2);
  return s;
}

sphere *make_static_sphere(arena *a, point3 centre1, scalar r, material *mat) {
  sphere *s = make_sphere(a, centre1, centre1, r, mat);
  vec3 rvec = (vec3){s->rad, s->rad, s->rad};
  init_aabb(vec3_sub(centre1, rvec), vec3_add(centre1, rvec), &(s->bbox));
  return s;
}
