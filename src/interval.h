#ifndef INTERVAL_H
#define INTERVAL_H
#include "utility.h"

typedef struct {
  scalar min;
  scalar max;
} interval;

extern const interval interval_empty;
extern const interval interval_universe;

static inline scalar interval_size(interval i) { return i.max - i.min; }

static inline int contains(interval i, double x) {
  return i.min <= x && x <= i.max;
}

static inline int surrounds(interval i, double x) {
  return i.min < x && x < i.max;
}

static inline interval empty_i() { return (interval){infinity, -infinity}; }

static scalar clamp(interval i, scalar x) {
  if (x < i.min)
    return i.min;
  if (x > i.max)
    return i.max;
  return x;
}

static interval expand(interval i, scalar delta) {
  scalar padding = delta / 2;
  return (interval){i.min - delta, i.max + delta};
}

static interval enclose_interval(interval a, interval b) {
  return (interval){.min = a.min <= b.min ? a.min : b.min,
                    .max = a.max >= b.max ? a.max : b.max};
}
#endif
