#include <stdio.h>
#include "vec3.h"


void vec3_print(FILE *out, vec3 a) {
    fprintf(out, "%lf %lf %lf\n", a.x, a.y, a.z);
}

vec3 vec3_random() {
        return (vec3){random_double(), random_double(), random_double()};
    }

vec3 vec3_random_interval(interval val) {
        return (vec3){random_double_range(val.min,val.max), random_double_range(val.min,val.max), random_double_range(val.min,val.max)};
    }

