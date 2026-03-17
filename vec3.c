#include <stdio.h>
#include "vec3.h"
#include "utility.h"


void vec3_print(FILE *out, vec3 a) {
    fprintf(out, "%lf %lf %lf\n", a.x, a.y, a.z);
}

vec3 vec3_random(pcg32_random_t *rng) {
        return (vec3){random_double(rng), random_double(rng), random_double(rng)};
    }

vec3 vec3_random_interval(interval val, pcg32_random_t *rng) {
        return (vec3){random_double_range(val.min,val.max, rng), random_double_range(val.min,val.max, rng), random_double_range(val.min,val.max, rng)};
    }

