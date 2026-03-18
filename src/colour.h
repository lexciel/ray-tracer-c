#ifndef COLOUR_H

#define COLOUR_H
#include "vec3.h"
#include <stdint.h>
#include <stdio.h>
typedef vec3 colour;
typedef struct {
    uint8_t r, g, b;
} pixel;
typedef struct {
    int height;
    int width;
    pixel *buffer;
} colour_buffer;
extern const colour white;
extern const colour black;
void colour_write(colour_buffer *buffer, colour c, pixel *index);
void colour_write_out(FILE *out, colour_buffer *buffer);
static inline double linear_to_gamma(double linear_component)
{
    if (linear_component > 0)
        return sqrt(linear_component);

    return 0;
}

#endif