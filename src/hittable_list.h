#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

typedef struct {
    hittable base;
    int count;
    int capacity;
    hittable *objects[]; // array of pointers to hittable objects, the objects themselves are either allocated on the stack or on arena allocator (later)
} hittable_list;

int hitlist_add(hittable_list *list, hittable *object);
int hit_list(const struct hittable *self, const ray *r, interval ray_t,  hit_record *rec);
void clear(hittable_list *list);
hittable_list* make_hitlist(arena *a, int capacity);

#endif