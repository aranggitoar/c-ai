#include <stdlib.h>
#include <time.h>
#include "utils.h"

// TODO: Implement minimum and maximum value.
#define RANDS(NAME, DTYPE, FUNC) \
DTYPE NAME(int seed) \
{ \
  srand(seed == 0 ? (int16_t)random() + (int16_t)time(NULL) : seed); \
  return FUNC; \
}
#define FRANDS_INNER(DTYPE) (rand() % 2 == 0) ? ((DTYPE)rand() / RAND_MAX) * -1 : (DTYPE)rand() / RAND_MAX
#define IRANDS_INNER(DTYPE) (rand() % 2 == 0) ? (DTYPE)rand() * -1 : (DTYPE)rand()

// For now floats are in the range of -1 to 1,
// and integers goes into tens of thousands.
RANDS(rand_f32, float, FRANDS_INNER(float))
RANDS(rand_f64, double, FRANDS_INNER(double))
RANDS(rand_i16, int16_t, IRANDS_INNER(int16_t))
RANDS(rand_i32, int32_t, IRANDS_INNER(int32_t))
RANDS(rand_i64, int64_t, IRANDS_INNER(int64_t))
