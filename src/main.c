#include <stdio.h>
#include "tensor.h"
#include "math.h"
#include "utils.h"

int main()
{
  printf("\n");

  int64_t shape[] = {2, 2, 3};

  float dataa[] = {-0.6889, -0.5507, -0.0514, 1.2568,  1.8930,
    0.2835, -0.2256,  0.6016, -0.0221, -0.0767,  0.6198,  1.2018};
  float datab[] = {-0.7054,  1.7017,  0.1438, -0.1890,  0.1546, -1.8488,
    -1.2197, -0.8214, -1.2325, -0.5310,  0.8262, -2.0328};

  /* Tensor a = tensor_create(LEN(shape), shape, F32, dataa); */
  /* Tensor b = tensor_create(LEN(shape), shape, F32, datab); */
  /* Tensor a = tensor_zero(LEN(shape), shape, I16); */
  /* Tensor b = tensor_zero(LEN(shape), shape, I16); */
  Tensor a = tensor_rand(LEN(shape), shape, F32);
  Tensor b = tensor_rand(LEN(shape), shape, F32);

  tensor_print_shape(&a);
  tensor_print_shape(&b);
  tensor_print(&a);
  tensor_print(&b);

  // Test tensor_reshape.
  /* int ns[] = {2, 6, 1}; */
  /* a = tensor_reshape(a, ns); */
  /* tensor_print_shape(a); */
  /* tensor_print(a); */

  // Test tensor_add/sub/mul/div.
  Tensor res = tensor_div(&a, &b);
  tensor_print_shape(&res);
  tensor_print(&res);
  tensor_free(&res);

  // Test tensor_dot.
  /* int ns[] = {2, 3, 2}; */
  /* b = tensor_reshape(&b, ns); */
  /* Tensor prod = tensor_dot(&a, &b); */
  // Kenapa 0,0,: - 0,1,: dan 1,0,: - 1,1,: sama?
  /* tensor_print_shape(&prod); */
  /* tensor_print(&prod); */
  /* tensor_free(&prod); */

  tensor_free(&a);
  tensor_free(&b);

  printf("\n");
  return 0;
}
