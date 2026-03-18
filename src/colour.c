#include <stdint.h>
#include <stdio.h>
#include "colour.h"
#include "interval.h"
void colour_write(colour_buffer *buffer, colour c, pixel *index) {
    scalar r = c.x;
    scalar g = c.y;
    scalar b = c.z;
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);


    const interval intensity = {0.000, 0.999};
    index->r = (uint8_t)(256 * clamp(intensity, r));
    index->g = (uint8_t)(256 * clamp(intensity, g));
    index->b = (uint8_t)(256 * clamp(intensity, b));

    

}

void colour_write_out(FILE *out, colour_buffer *buffer) {

    for (int i = 0; i < buffer->height; i++) {
        for (int j = 0; j < buffer->width; j++) {
            
            fprintf(out, "%d %d %d\n", buffer->buffer[i * buffer->width + j].r, buffer->buffer[i * buffer->width + j].g,buffer->buffer[i * buffer->width + j].b );
        }
    }
}
const colour white = {1,1,1};
const colour black = {0, 0, 0};