#ifndef BVH_H
#define BVH_H

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"
#include "interval.h"
#include <sys/types.h>

typedef struct {
  hittable hit;
  hittable *right;
  hittable *left;
  aabb bbox;
} bvh_node;

typedef int (*hittable_cmp)(const void *, const void *);

int hit_bvh(const struct hittable *self, const ray *r, interval ray_t,
            hit_record *rec);

static int box_compare(const void *a, const void *b, u_int32_t axis);
static int box_x_compare(const void *a, const void *b);
static int box_y_compare(const void *a, const void *b);
static int box_z_compare(const void *a, const void *b);

aabb bounding_box_bvh(hittable *self);
bvh_node *make_bvh_hlist(hittable_list *list, arena *arena);
bvh_node *init_bvh_node(bvh_node *node, hittable **objects, size_t start,
                        size_t end, arena *arena);

#endif
