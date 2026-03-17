#ifndef INTERVAL_H
#define INTERVAL_H
#include "utility.h"

typedef struct {
    scalar min;
    scalar max;
} interval;

static inline scalar size(interval i) {
    return i.max - i.min;
}

static inline int contains(interval i, double x) {
        return i.min <= x && x <= i.max;
    }

static inline int surrounds(interval i, double x) {
        return i.min < x && x < i.max;
    }

static inline interval empty_i() {
    return (interval){infinity, - infinity};
}

static scalar clamp(interval i, scalar x)  {
        if (x < i.min) return i.min;
        if (x > i.max) return i.max;
        return x;
    }
#endif