#include <stdlib.h>

typedef enum {
    I16,
    I32,
    I64,
    F32,
    F64,
} tensor_dtype;

typedef enum {
    ADD,
    SUB,
    MUL,
    DIV,
} elementwise_op;

typedef struct {
    int64_t numDims;
    int64_t *shape;
    tensor_dtype dtype;
    void *data;
} Tensor;

Tensor tensor_create(int64_t nd, int64_t *shape, tensor_dtype dtype, void *data);
Tensor tensor_zero(int64_t nd, int64_t *shape, tensor_dtype dtype);
Tensor tensor_rand(int64_t nd, int64_t *shape, tensor_dtype dtype);
void tensor_free(Tensor *t);

void tensor_print(Tensor *t);
void tensor_print_shape(Tensor *t);

int tensor_are_compatible(Tensor *a, Tensor *b);
int64_t tensor_get_size(int64_t nd, int64_t *shape);
int tensor_bitsize(tensor_dtype dtype);
Tensor tensor_reshape(Tensor *t, int64_t *newShape);
int64_t tensor_get_flattened_index(Tensor *t, int64_t *indices);
Tensor tensor_get_element(Tensor *t, int64_t *coords);

Tensor tensor_add(Tensor *a, Tensor *b);
Tensor tensor_sub(Tensor *a, Tensor *b);
Tensor tensor_mul(Tensor *a, Tensor *b);
Tensor tensor_div(Tensor *a, Tensor *b);
Tensor tensor_dot(Tensor *a, Tensor *b);
