#include "sphere.h"
#include "hittable.h"
#include "interval.h"
#include "vec3.h"
#include <math.h>   

int hit_sphere(const hittable *self, const ray *r, interval ray_t, hit_record *rec) {
    const sphere *s = (const sphere*) self;
    vec3 oc = vec3_sub(s->centre, r->orig);
    scalar a = vec3_length_squared(r->dir);
    scalar h = vec3_dot(r->dir, oc);
    scalar c = vec3_length_squared(oc) - s->rad*s->rad;
    scalar disc = h*h - a*c;

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
    
    vec3 outward_normal = vec3_scale(vec3_sub(rec->p, s->centre), 1.0/s->rad);
    set_face_normal(rec, r, &outward_normal);
    rec->mat = s->mat;


    return 1;


}


sphere *make_sphere(arena *a, point3 c, scalar r, material *mat) {
    sphere *s = arena_alloc(a, sizeof(sphere));
    s->centre = c;
    s->rad = fmax(0, r);
    s->base.hit = hit_sphere;
    s->mat = mat;
    return s;
}