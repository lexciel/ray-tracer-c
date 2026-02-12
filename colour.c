#include <stdio.h>
#include "colour.h"
#include "interval.h"
void colour_write(FILE *out, colour c) {
    scalar r = c.x;
    scalar g = c.y;
    scalar b = c.z;
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);


    const interval intensity = {0.000, 0.999};
    int rbyte = (int)(256 * clamp(intensity, r));
    int gbyte = (int)(256 * clamp(intensity, g));
    int bbyte = (int)(256 * clamp(intensity, b));
    fprintf(out, "%d %d %d\n", rbyte, gbyte, bbyte);

}

const colour white = {1,1,1};
const colour black = {0, 0, 0};