#ifndef UTILITY_H

#define UTILITY_H
#include "arena.h"
#include <float.h>
#include <math.h>
#include <stdlib.h> //rand() and RAND_MAX

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
typedef struct {
  uint64_t state;
  uint64_t inc;
} pcg32_random_t;

uint32_t pcg32_random_r(pcg32_random_t *rng);
void pcg32_seed(pcg32_random_t *rng, uint64_t seed, uint64_t stream);
double random_double(pcg32_random_t *rng);
// Returns a random real in [min,max).
static inline double random_double_range(double min, double max,
                                         pcg32_random_t *rng) {
  return min + (max - min) * random_double(rng);
}

#endif
