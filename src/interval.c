#include "interval.h"

const interval interval_empty = {.min = INFINITY, .max = -INFINITY};
const interval interval_universe = {.min = -INFINITY, .max = INFINITY};
