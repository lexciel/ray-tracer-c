#ifndef BVH_H
#define BVH_H

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"

typedef struct {
  hittable hit;
  hittable *right;
  hittable *left;
  aabb bbox;
} bvh_node;

int hit_bvh(struct hittable *self, const ray *r, interval ray_t,
            hit_record *rec) {
  bvh_node *node = (bvh_node *)self;
  if (!bb_hit(&node->bbox, r, ray_t)) {
    return 0;
  }

  int hit_left = node->left->hit(node->left, r, ray_t, rec);
  int hit_right = node->right->hit(node->right, r, ray_t, rec);

  return hit_left || hit_right;
}

aabb bounding_box_bvh(hittable *self) { return ((bvh_node *)self)->bbox; }
bvh_node *make_bvh_hlist(hittable_list *list);
bvh_node *init_bvh_node(hittable *objects, size_t start, size_t end);
#endif
