#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "tensor.h"
#include "math.h"

int tensor_are_compatible(Tensor *a, Tensor *b)
{
  // Broadcasting for scalars?
  if (a->dtype != b->dtype) return 0; // Different type
  if (a->numDims != b->numDims) return 0; // Different number of dimensions
  for (int64_t i = 0; i < a->numDims; i++)
    if (a->shape[i] != b->shape[i]) return 0; // Different shape in dimension i
  return 1; // Tensors are compatible
}

int64_t tensor_get_size(int64_t nd, int64_t *shape)
{
  int64_t size = 1;
  for (int64_t i = 0; i < nd; i++) size *= shape[i];
  return size;
}

int tensor_bitsize(tensor_dtype dtype)
{
  switch(dtype) {
    case I16: return 2;
    case I32:
    case F32: return 4;
    case I64:
    case F64: return 8;
  }
}

void tensor_free(Tensor *t)
{
  free(t->shape), free(t->data);
}

Tensor tensor_reshape(Tensor *t, int64_t *newShape)
{
  memcpy(t->shape, newShape, t->numDims * sizeof(int64_t));
  return *t;
}

int64_t tensor_get_flattened_index(Tensor *t, int64_t *indices)
{
  int64_t index = 0, stride = 1;
  for (int64_t i = t->numDims - 1; i >= 0; --i) {
      index += indices[i] * stride, stride *= t->shape[i];
  }
  return index;
}

#define DEFINE_TENSOR_PRINT(NAME, FORMAT, DTYPE)                              \
static void NAME(Tensor *t)                                                   \
{                                                                             \
  int64_t size = tensor_get_size(t->numDims, t->shape);                       \
  for (int64_t i = 0; i < size; i++)                                          \
    printf(i == 0 ? "\n"FORMAT"\n" : i == size-1 ? FORMAT"\n\n" : FORMAT"\n", \
        ((DTYPE*)t->data)[i]);                                                \
}

DEFINE_TENSOR_PRINT(print_i16, "%hi", int16_t);
DEFINE_TENSOR_PRINT(print_i32, "%i", int32_t);
DEFINE_TENSOR_PRINT(print_i64, "%li", int64_t);
DEFINE_TENSOR_PRINT(print_f32, "%f", float);
DEFINE_TENSOR_PRINT(print_f64, "%f", double);

// Print a tensor based on its shape not size.
void tensor_print(Tensor *t)
{
  switch(t->dtype) {
    case I16: print_i16(t); break;
    case I32: print_i32(t); break;
    case I64: print_i64(t); break;
    case F32: print_f32(t); break;
    case F64: print_f64(t); break;
  }
}

void tensor_print_shape(Tensor *t)
{
  for (int64_t i = 0; i < t->numDims; i++)
    printf(i == 0 ? "\n{%li " : i == t->numDims-1 ? "%li}\n\n" : "%li ", t->shape[i]);
}

/* float tensor_get_element(Tensor *tensor, int *coords) { */
/*     int index = 0, stride = 1; */
/*     for (int d = tensor->numDims - 1; d >= 0; d--) { */
/*         index += coords[d] * stride; */
/*         stride *= tensor->shape[d]; */
/*     } */
/*     return tensor->data[index]; */
/* } */

Tensor tensor_get_element(Tensor *t, int64_t *coords)
{
  int64_t index = 0, stride = 1;
  for (int64_t d = t->numDims - 1; d >= 0; d--) {
    index += coords[d] * stride, stride *= t->shape[d];
  }
  Tensor nt = {
    .numDims = 1,
    .shape = (int64_t[]){1},
    .dtype = t->dtype,
    .data = malloc(1 * tensor_bitsize(t->dtype)), // as shape is {1}
  };
  switch(t->dtype) {
    case I16: ((int16_t*)nt.data)[0] = ((int16_t*)t->data)[index]; break;
    case I32: ((int32_t*)nt.data)[0] = ((int32_t*)t->data)[index]; break;
    case I64: ((int64_t*)nt.data)[0] = ((int64_t*)t->data)[index]; break;
    case F32: ((float*)nt.data)[0] = ((float*)t->data)[index]; break;
    case F64: ((double*)nt.data)[0] = ((double*)t->data)[index]; break;
  }

  return nt;
}

Tensor tensor_create(int64_t nd, int64_t *shape, tensor_dtype dtype, void *data)
{
  int64_t ssz = nd * sizeof(int64_t), dsz = tensor_get_size(nd, shape) * tensor_bitsize(dtype);
  Tensor t = {
    .numDims = nd,
    .shape = (int64_t *)malloc(ssz),
    /* .shape = shape, */
    .dtype = dtype,
    .data = malloc(dsz)
  };
  if (t.data == NULL) printf("Memory allocation failed.\n");
  memcpy(t.shape, shape, ssz);
  memcpy(t.data, data, dsz);
  return t;
}

Tensor tensor_zero(int64_t nd, int64_t *shape, tensor_dtype dtype)
{
  int dsz = tensor_get_size(nd, shape);
  void *data = malloc(dsz * tensor_bitsize(dtype));
  for (int i = 0; i < dsz; i++) ((int16_t*)data)[i] = 0;
  return tensor_create(nd, shape, dtype, data);
}

#define DEFINE_TENSOR_RAND(NAME, DTYPE)                                     \
static Tensor trand_##NAME(int64_t nd, int64_t *shape, tensor_dtype dtype)  \
{                                                                           \
  int dsz = tensor_get_size(nd, shape);                                     \
  void *data = malloc(dsz * tensor_bitsize(dtype));                         \
  for (int i = 0; i < dsz; i++) ((DTYPE *)data)[i] = rand_##NAME(0);        \
  return tensor_create(nd, shape, dtype, data);                             \
}

DEFINE_TENSOR_RAND(i16, int16_t)
DEFINE_TENSOR_RAND(i32, int32_t)
DEFINE_TENSOR_RAND(i64, int64_t)
DEFINE_TENSOR_RAND(f32, float)
DEFINE_TENSOR_RAND(f64, double)

Tensor tensor_rand(int64_t nd, int64_t *shape, tensor_dtype dtype)
{
  switch(dtype) {
    case I16: return trand_i16(nd, shape, dtype);
    case I32: return trand_i32(nd, shape, dtype);
    case I64: return trand_i64(nd, shape, dtype);
    case F32: return trand_f32(nd, shape, dtype);
    case F64: return trand_f64(nd, shape, dtype);
  }
}

#define DEFINE_TENSOR_OP(NAME, DTYPE, OP)                                       \
static Tensor NAME(Tensor *a, Tensor *b) {                                      \
  int totalElements = tensor_get_size(a->numDims, a->shape);                    \
  Tensor res = {                                                                \
    .numDims = a->numDims,                                                      \
    .shape = (int64_t*)malloc(res.numDims * sizeof(int64_t)),                   \
    .dtype = a->dtype,                                                          \
    .data = malloc(totalElements * tensor_bitsize(res.dtype))                   \
  };                                                                            \
  memcpy(res.shape, a->shape, a->numDims * sizeof(int64_t));                    \
  int64_t *indices = (int64_t*)malloc(res.numDims * sizeof(int64_t));           \
  memset(indices, 0, res.numDims * sizeof(int64_t));                            \
  for (int64_t i = 0; i < totalElements; i++) {                                 \
    int64_t temp = i;                                                           \
    for (int d = res.numDims - 1; d >= 0; --d) {                                \
      indices[d] = temp % res.shape[d];                                         \
      temp /= res.shape[d];                                                     \
    }                                                                           \
    int fi = tensor_get_flattened_index(&res, indices);                         \
    ((DTYPE*)res.data)[fi] = ((DTYPE*)a->data)[fi] OP ((DTYPE*)b->data)[fi];    \
  }                                                                             \
  free(indices);                                                                \
  return res;                                                                   \
}

DEFINE_TENSOR_OP(i16_add, int16_t, +);
DEFINE_TENSOR_OP(i16_sub, int16_t, -);
DEFINE_TENSOR_OP(i16_mul, int16_t, *);
DEFINE_TENSOR_OP(i16_div, int16_t, /);

DEFINE_TENSOR_OP(i32_add, int32_t, +);
DEFINE_TENSOR_OP(i32_sub, int32_t, -);
DEFINE_TENSOR_OP(i32_mul, int32_t, *);
DEFINE_TENSOR_OP(i32_div, int32_t, /);

DEFINE_TENSOR_OP(i64_add, int64_t, +);
DEFINE_TENSOR_OP(i64_sub, int64_t, -);
DEFINE_TENSOR_OP(i64_mul, int64_t, *);
DEFINE_TENSOR_OP(i64_div, int64_t, /);

DEFINE_TENSOR_OP(f32_add, float, +);
DEFINE_TENSOR_OP(f32_sub, float, -);
DEFINE_TENSOR_OP(f32_mul, float, *);
DEFINE_TENSOR_OP(f32_div, float, /);

DEFINE_TENSOR_OP(f64_add, double, +);
DEFINE_TENSOR_OP(f64_sub, double, -);
DEFINE_TENSOR_OP(f64_mul, double, *);
DEFINE_TENSOR_OP(f64_div, double, /);

static Tensor i16_ops(Tensor *a, Tensor *b, elementwise_op op) {
  switch(op) {
    case ADD: return i16_add(a, b);
    case SUB: return i16_sub(a, b);
    case MUL: return i16_mul(a, b);
    case DIV: return i16_add(a, b);
  }
}

static Tensor i32_ops(Tensor *a, Tensor *b, elementwise_op op) {
  switch(op) {
    case ADD: return i32_add(a, b);
    case SUB: return i32_sub(a, b);
    case MUL: return i32_mul(a, b);
    case DIV: return i32_add(a, b);
  }
}

static Tensor i64_ops(Tensor *a, Tensor *b, elementwise_op op) {
  switch(op) {
    case ADD: return i64_add(a, b);
    case SUB: return i64_sub(a, b);
    case MUL: return i64_mul(a, b);
    case DIV: return i64_add(a, b);
  }
}

static Tensor f32_ops(Tensor *a, Tensor *b, elementwise_op op) {
  switch(op) {
    case ADD: return f32_add(a, b);
    case SUB: return f32_sub(a, b);
    case MUL: return f32_mul(a, b);
    case DIV: return f32_add(a, b);
  }
}

static Tensor f64_ops(Tensor *a, Tensor *b, elementwise_op op) {
  switch(op) {
    case ADD: return f64_add(a, b);
    case SUB: return f64_sub(a, b);
    case MUL: return f64_mul(a, b);
    case DIV: return f64_add(a, b);
  }
}

static Tensor basic_ops(Tensor *a, Tensor *b, elementwise_op op) {
  if (!tensor_are_compatible(a, b)) {
    printf("Tensors incompatible."); // TODO: better error handling
    exit(1);
  }

  switch(a->dtype) {
    case I16: return i16_ops(a, b, op);
    case I32: return i32_ops(a, b, op);
    case I64: return i64_ops(a, b, op);
    case F32: return f32_ops(a, b, op);
    case F64: return f64_ops(a, b, op);
  }
}

Tensor tensor_add(Tensor *a, Tensor *b) {
  return basic_ops(a, b, ADD);
}

Tensor tensor_sub(Tensor *a, Tensor *b) {
  return basic_ops(a, b, SUB);
}

Tensor tensor_mul(Tensor *a, Tensor *b) {
  return basic_ops(a, b, MUL);
}

Tensor tensor_div(Tensor *a, Tensor *b) {
  return basic_ops(a, b, DIV);
}

Tensor tensor_dot(Tensor *a, Tensor *b) {
  if (a->shape[a->numDims - 1] != b->shape[b->numDims - 2]) {
    printf("Tensors incompatible."); // TODO: proper error handling, should be a different case with mul()
    exit(1);
  }

  // Determine the resulting tensor's shape and dimensions,
  // for now only works for equal dimensions
  int64_t resultNumDims = a->numDims;
  int64_t *resultShape = (int64_t *)malloc(resultNumDims * sizeof(int64_t));
  memcpy(resultShape, a->shape, resultNumDims * sizeof(int64_t));
  for (int64_t i = 0; i < b->numDims - 1; i++)
    // Skip the second-to-last dimension of b
    if (i != b->numDims - 2) resultShape[a->numDims - 1 + i] = b->shape[i];

  // Allocate the resulting tensor
  Tensor result;
  result.numDims = resultNumDims;
  result.shape = resultShape;
  int64_t totalResultElements = tensor_get_size(result.numDims, result.shape);
  result.dtype = a->dtype;
  result.data = malloc(totalResultElements * tensor_bitsize(result.dtype));

  // Compute the dot product
  int commonDim = a->shape[a->numDims - 1];
  for (int i = 0; i < totalResultElements; i++) {
    int *resultIndices = (int *)malloc(resultNumDims * sizeof(int));
    int tempI = i;
    for (int d = resultNumDims - 1; d >= 0; d--) {
      resultIndices[d] = tempI % resultShape[d];
      tempI /= resultShape[d];
    }

    float sum = 0.0f;
    for (int k = 0; k < commonDim; k++) {
      int aIndex = 0, bIndex = 0, stride = 1;
      for (int d = a->numDims - 2; d >= 0; d--) {
        aIndex += (d == a->numDims - 2 ? k : resultIndices[d]) * stride;
        stride *= a->shape[d];
      }
      stride = 1;
      for (int d = b->numDims - 1; d >= 0; d--) {
        if (d != b->numDims - 2) {
          bIndex += (d == b->numDims - 1 ? resultIndices[a->numDims - 2 + d - 1] : (d < b->numDims - 2 ? resultIndices[a->numDims - 1 + d] : k)) * stride;
          stride *= b->shape[d];
        }
      }

      /* sum += a->data[aIndex] * b->data[bIndex]; */
      sum += ((float*)a->data)[aIndex] * ((float*)b->data)[bIndex];
      printf("at result index of %d and rev. iter %d of the commonDim, sum is %f\n", i, k, sum);
      printf("it takes index %d of (a) and index %d of (b)\n", aIndex, bIndex);
    }
    printf("\n");

    /* printf("%f\n", sum); */
    ((float*)result.data)[i] = sum;
    free(resultIndices);
  }

  return result;
}
