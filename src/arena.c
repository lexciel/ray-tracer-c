#include "arena.h"

arena arena_init(size_t size) {
    arena a;
    a.buffer = (unsigned char*)malloc(size);
    a.buffer_len = size;
    a.offset = 0;
    return a;
}

void *arena_alloc(arena *a, size_t size) {

    uintptr_t current_ptr = (uintptr_t)a->buffer + (uintptr_t)a->offset;
    size_t padding = 0;
    if ((current_ptr % ALIGNMENT)) {
        padding = ALIGNMENT - (current_ptr % ALIGNMENT);
    }

    if (a->buffer_len < a->offset + size + padding) {
        return NULL;
    }

    a->offset += padding;
    void *ptr = &a->buffer[a->offset];
    a->offset += size;
    memset(ptr, 0, size);
    return ptr;
}

void arena_free(arena *a) {
    free(a->buffer);
    a->buffer = NULL;
    a->buffer_len = 0;
    a->offset = 0;
}

void arena_reset(arena *a) {
    a->offset = 0;
}
