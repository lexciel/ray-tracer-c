#ifndef COLOUR_H

#define COLOUR_H
#include "vec3.h"
#include <stdio.h>
typedef vec3 colour;
extern const colour white;
extern const colour black;
void colour_write(FILE *out, colour c);
static inline double linear_to_gamma(double linear_component)
{
    if (linear_component > 0)
        return sqrt(linear_component);

    return 0;
}

#endif