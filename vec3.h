#ifndef VEC3_H

#define VEC3_H
#include <math.h>
#include <stdio.h>
#include <float.h> 
#include "interval.h"
#include "utility.h"




typedef union {
    scalar v[3];

    struct {
        scalar x;
        scalar y;
        scalar z;
    };
    
} vec3 ;

typedef vec3 point3;
static inline vec3 vec3_add(vec3 a, vec3 b) {
    return (vec3){a.x+b.x, a.y+b.y, a.z+b.z};
}

static inline vec3 vec3_sub(vec3 a, vec3 b) {
    return (vec3){a.x-b.x, a.y-b.y, a.z-b.z};
}
    
static inline vec3 vec3_scale(vec3 a, scalar m) {
    return (vec3){m*a.x, m*a.y, m*a.z};
}
static inline vec3 vec3_mult(vec3 a, vec3 b) {
    return (vec3){a.x*b.x, a.y*b.y, a.z*b.z};
}
static inline scalar vec3_dot(vec3 a, vec3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z ;
}

static inline scalar vec3_length_squared(vec3 a){
    return a.x*a.x + a.y*a.y + a.z*a.z;
}
static inline scalar vec3_length(vec3 a) {
    return sqrt(vec3_length_squared(a));
}

static inline vec3 vec3_unit(vec3 a) {
    double len = vec3_length(a);
    double zero_case = (double)(len > VEC3_EPSILON);
    return vec3_scale(a, zero_case/(len + 1.0 - zero_case));
}
static inline vec3 vec3_cross(vec3 a, vec3 b) {
    vec3 vec;
    vec.x = a.y*b.z - b.y*a.z;
    vec.y = a.z*b.x - b.z*a.x;
    vec.z = a.x*b.y - b.x*a.y;
    return vec;
};

void vec3_print(FILE *out, vec3 a);


vec3 vec3_random();
vec3 vec3_random_interval(interval val); 

static inline vec3 vec3_random_unit_vector() {
    while (1) {
        vec3 p = vec3_random_interval((interval){-1, 1});
        scalar lensq = vec3_length_squared(p);
        if (VEC3_EPSILON < lensq && lensq <= 1)
            return vec3_scale(p , 1.0 / sqrt(lensq));
    }
}

static inline vec3 vec3_random_on_hemisphere(vec3 normal) {
    vec3 on_unit_sphere = vec3_random_unit_vector();
    if (vec3_dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return vec3_scale(on_unit_sphere, -1);
}

static inline int vec3_near_zero(vec3 v) {
    const double s = 1e-8;
    return (fabs(v.x) < s) && (fabs(v.y) < s) && (fabs(v.z) < s);
}

static inline vec3 vec3_reflect(vec3 v, vec3 n) {
    return vec3_sub(v, vec3_scale(n, 2*vec3_dot(v,n)));
}

static inline vec3 vec3_refract(const vec3 uv, const vec3 n, scalar etai_over_etat) {
    scalar cos_theta = fmin(vec3_dot(vec3_scale(uv, -1), n), 1.0);
    vec3 r_out_perp =  vec3_scale(vec3_add(uv , vec3_scale(n, cos_theta)), etai_over_etat );
    vec3 r_out_parallel = vec3_scale(n, -sqrt(fabs(1.0 - vec3_length_squared(r_out_perp)))) ;
    return vec3_add(r_out_perp , r_out_parallel);
}

static inline vec3 vec3_random_in_unit_disk() {
    while(1) {
        vec3 p = {random_double_range(-1, 1), random_double_range(-1, 1), 0};
        if (vec3_length_squared(p) < 1) {
            return p;
        }
    }
}
#endif