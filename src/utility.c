#include "utility.h"
#include <stdint.h>

const double infinity = DBL_MAX;
const double pi = 3.1415926535897932385;

uint32_t pcg32_random_r(pcg32_random_t *rng) {
  uint64_t oldstate = rng->state;
  // Advance internal state
  rng->state = oldstate * 6364136223846793005ULL + (rng->inc | 1);
  // Calculate output function (XSH RR), uses old state for max ILP
  uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
  uint32_t rot = oldstate >> 59u;
  return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

double random_double(pcg32_random_t *rng) {
  return (double)pcg32_random_r(rng) * (1.0 / 4294967296.0);
}

void pcg32_seed(pcg32_random_t *rng, uint64_t seed, uint64_t stream) {
  rng->state = 0;
  rng->inc = (stream << 1) | 1; // guarantees odd
  pcg32_random_r(rng);          // step once
  rng->state += seed;
  pcg32_random_r(rng); // step again
}
