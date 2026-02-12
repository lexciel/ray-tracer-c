#include "utility.h"

const double infinity = DBL_MAX;
const double pi = 3.1415926535897932385;
// 2. A fast, custom RNG (PCG Hash or Xorshift)
// Don't use rand()!
uint32_t pcg_hash(uint32_t input) {
    uint32_t state = input * 747796405u + 2891336453u;
    uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}

double random_double() {
    // Determine a seed if this thread hasn't initialized yet
    if (my_seed == 0) {
        // Use address of a local var + time to create unique seed per thread
        uintptr_t p = (uintptr_t)&my_seed; 
        my_seed = pcg_hash((uint32_t)p); 
    }

    // Xorshift algorithm (Very fast)
    my_seed ^= my_seed << 13;
    my_seed ^= my_seed >> 17;
    my_seed ^= my_seed << 5;

    // Convert to double [0, 1)
    return my_seed * (1.0 / 4294967296.0);
}