#include "hittable_list.h"

#include <stdlib.h>

int hitlist_add(hittable_list *list, hittable *object) {
    if (list->count < list->capacity) {
        list->objects[list->count++] = object;
        return 1;
    } 
    return 0;
}

int hit_list(const struct hittable *self, const ray *r, interval ray_t, hit_record *rec) {
    hit_record temp_rec;
    int hit_anything = 0;
    scalar closest_so_far = ray_t.max;
    hittable_list *list = (hittable_list*) self;

    int count = 0;
    int total = list->count;
    

    for (int i = 0; i < total; i++) {
        hittable *p = list->objects[i];
        
        if (p->hit(p , r, (interval){ray_t.min, closest_so_far}, &temp_rec)) {
            hit_anything = 1;
            closest_so_far = temp_rec.t;
            *rec = temp_rec;
        }
    }

    return hit_anything;

}
void clear(hittable_list *list) {
    free(list);
}

hittable_list* make_hitlist(arena *a, int capacity) {

    size_t total_size = sizeof(hittable_list) + sizeof(hittable*) * capacity;
    hittable_list *list = arena_alloc(a, total_size);

    list->count = 0;
    list->capacity = capacity;
    list->base.hit = hit_list;

    return list;
}
