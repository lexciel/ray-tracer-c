#ifndef ARENA_H
#define ARENA_H
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define ALIGNMENT 64

typedef struct {
    unsigned char *buffer;
    size_t buffer_len;
    size_t offset;
} arena;

arena arena_init(size_t size) ;

void *arena_alloc(arena *a, size_t size) ;

void arena_free(arena *a) ;

void arena_reset(arena *a) ;
#endif