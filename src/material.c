#include "material.h"
#include "utility.h"
#include "vec3.h"

int scatter_lambertian(const material *self, const ray *ray_in,
                       const hit_record *rec, colour *attentuation,
                       ray *scattered, pcg32_random_t *rng) {
  const lambertian *this = (lambertian *)self;
  vec3 scatter_direction = vec3_add(rec->normal, vec3_random_unit_vector(rng));

  if (vec3_near_zero(scatter_direction))
    scatter_direction = rec->normal;

  *scattered = (ray){rec->p, scatter_direction, ray_time(ray_in)};
  *attentuation = this->albedo;
  return 1;
}

int scatter_metal(const material *self, const ray *ray_in,
                  const hit_record *rec, colour *attentuation, ray *scattered,
                  pcg32_random_t *rng) {
  const metal *this = (metal *)self;
  vec3 reflected = vec3_reflect(ray_in->dir, rec->normal);
  reflected = vec3_add(vec3_unit(reflected),
                       vec3_scale(vec3_random_unit_vector(rng), this->fuzz));
  *scattered = (ray){rec->p, reflected, ray_time(ray_in)};
  *attentuation = this->albedo;
  return vec3_dot(scattered->dir, rec->normal) > 0;
}

int scatter_dialectric(const material *self, const ray *ray_in,
                       const hit_record *rec, colour *attentuation,
                       ray *scattered, pcg32_random_t *rng) {
  const dialectric *this = (dialectric *)self;
  *attentuation = white;
  scalar ri =
      rec->front_face ? (1.0 / this->refraction_index) : this->refraction_index;

  vec3 unit_direction = vec3_unit(ray_in->dir);
  scalar cos_theta =
      fmin(vec3_dot(vec3_scale(unit_direction, -1), rec->normal), 1.0);
  scalar sin_theta = sqrt(1.0 - cos_theta * cos_theta);

  int cannot_refract = ri * sin_theta > 1.0;
  vec3 direction;
  if (cannot_refract || reflectance(cos_theta, ri) > random_double(rng)) {
    direction = vec3_reflect(unit_direction, rec->normal);
  } else {
    direction = vec3_refract(unit_direction, rec->normal, ri);
  }

  *scattered = (ray){rec->p, direction, ray_time(ray_in)};
  return 1;
}
lambertian *make_lambertian(arena *a, colour albedo) {

  lambertian *l = arena_alloc(a, sizeof(lambertian));
  l->albedo = albedo;
  l->mat.scatter = scatter_lambertian;
  return l;
}

metal *make_metal(arena *a, colour albedo, scalar fuzz) {

  metal *m = arena_alloc(a, sizeof(metal));
  m->albedo = albedo;
  m->mat.scatter = scatter_metal;
  m->fuzz = fmin(1, fuzz);

  return m;
}

dialectric *make_dialectric(arena *a, scalar refraction_index) {
  dialectric *d = arena_alloc(a, sizeof(dialectric));
  d->refraction_index = refraction_index;
  d->mat.scatter = scatter_dialectric;
  return d;
}
