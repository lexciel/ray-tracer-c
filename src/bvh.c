#include "bvh.h"
#include "aabb.h"
#include "arena.h"
#include "hittable.h"
#include "utility.h"
#include <stddef.h>
#include <sys/types.h>

int hit_bvh(const struct hittable *self, const ray *r, interval ray_t,
            hit_record *rec) {
  bvh_node *node = (bvh_node *)self;
  if (!bb_hit(&node->bbox, r, ray_t)) {
    return 0;
  }

  int hit_left = node->left->hit(node->left, r, ray_t, rec);
  ray_t.max = hit_left ? rec->t : ray_t.max;
  int hit_right = node->right->hit(node->right, r, ray_t, rec);

  return hit_left || hit_right;
}

aabb bounding_box_bvh(hittable *self) { return ((bvh_node *)self)->bbox; }

static int box_compare(const void *a, const void *b, u_int32_t axis) {
  const hittable *ha = *(const hittable *const *)a;
  const hittable *hb = *(const hittable *const *)b;
  interval ia = ha->bbox.box[axis];
  interval ib = hb->bbox.box[axis];
  return (ia.min > ib.min) - (ia.min < ib.min);
}

static int box_x_compare(const void *a, const void *b) {
  return box_compare(a, b, 0);
}
static int box_y_compare(const void *a, const void *b) {
  return box_compare(a, b, 1);
}
static int box_z_compare(const void *a, const void *b) {
  return box_compare(a, b, 2);
}

bvh_node *make_bvh_hlist(hittable_list *list, arena *arena) {
  bvh_node *node = arena_alloc(arena, sizeof(bvh_node));
  node->hit.bb = bounding_box_bvh;
  node->hit.hit = hit_bvh;
  hittable **copy = malloc(list->count * sizeof(*copy));
  memcpy(copy, list->objects, list->count * sizeof(*copy));

  init_bvh_node(node, copy, 0, list->count, arena);
  free(copy);
  return node;
}
bvh_node *init_bvh_node(bvh_node *node, hittable **objects, size_t start,
                        size_t end, arena *arena) {
  node->hit.bb = bounding_box_bvh;
  node->hit.hit = hit_bvh;
  node->bbox = aabb_empty;

  for (size_t object_index = start; object_index < end; object_index++) {
    node->bbox = enclose_aabb(node->bbox,
                              objects[object_index]->bb(objects[object_index]));
  }

  int axis = longest_axis(node->bbox);
  hittable_cmp comparator = (axis == 0)   ? box_x_compare
                            : (axis == 1) ? box_y_compare
                                          : box_z_compare;

  size_t object_span = end - start;

  if (object_span == 1) {
    node->left = node->right = objects[start];
  } else if (object_span == 2) {
    node->left = objects[start];
    node->right = objects[start + 1];
  } else {
    qsort(objects + start, end - start, sizeof(*objects), comparator);
    size_t mid = start + object_span / 2;

    bvh_node *left = arena_alloc(arena, sizeof(bvh_node));
    bvh_node *right = arena_alloc(arena, sizeof(bvh_node));
    node->left = (hittable *)init_bvh_node(left, objects, start, mid, arena);
    node->right = (hittable *)init_bvh_node(right, objects, mid, end, arena);
  }

  return node;
}
