#ifndef MATERIAL_H
#define MATERIAL_H
#include "hittable.h"
#include "utility.h"
#include "colour.h"

typedef int (*scatter_fn)(const material* self, const ray *ray_in, const hit_record *rec, colour *attentuation, ray *scattered, pcg32_random_t *rng);

struct material {
    scatter_fn scatter;
}  ;

typedef struct {
    material mat;
    colour albedo;
} lambertian;

typedef struct {
    material mat;
    colour albedo;
    scalar fuzz;
} metal;

typedef struct {
    material mat;
    scalar refraction_index;
} dialectric;

int scatter_lambertian(const material* self, const ray *ray_in,const hit_record *rec, colour *attentuation, ray *scattered, pcg32_random_t *rng);
int scatter_metal(const material* self, const ray *ray_in,const hit_record *rec, colour *attentuation, ray *scattered, pcg32_random_t *rng);
int scatter_dialectric(const material* self, const ray *ray_in,const hit_record *rec, colour *attentuation, ray *scattered, pcg32_random_t *rng);

lambertian* make_lambertian(arena* a, colour albedo);
metal* make_metal(arena* a, colour albedo, scalar fuzz);
dialectric* make_dialectric(arena* a, scalar refraction_index);

static inline scalar reflectance(scalar cosine, scalar refraction_index) {
    scalar r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 *= r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}
#endif