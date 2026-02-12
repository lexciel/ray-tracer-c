#ifndef UTILITY_H

#define UTILITY_H
#include <math.h>
#include <float.h>
#include <stdlib.h> //rand() and RAND_MAX
#include "arena.h"

#ifdef USE_FLOAT_PRECISION
    typedef float scalar;
    #define VEC3_EPSILON FLT_EPSILON
#else
    typedef double scalar;
    #define VEC3_EPSILON DBL_EPSILON
#endif

extern const double infinity;
extern const double pi;

static inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

static _Thread_local uint32_t my_seed = 0; 

// 2. A fast, custom RNG (PCG Hash or Xorshift)
// Don't use rand()!
uint32_t pcg_hash(uint32_t input);

double random_double();
// Returns a random real in [min,max).
static inline double random_double_range(double min, double max) {
    return min + (max - min) * random_double();
}



#endif