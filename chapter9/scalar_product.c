#include <stdio.h>

const int vec_a[] = {1,2,3};
const int vec_b[] = {4,5,6};

int array_sum( const int* l_vec, const int* r_vec, size_t count ) {
  size_t i;
  int sum = 0;

  for( i = 0; i < count; i++ )
    sum = sum + (l_vec[i] * r_vec[i]);
  return sum;
}

int main( int argc, char** argv ) {
  printf(
      "The scalar product is: %d\n",
      array_sum(vec_a, vec_b , sizeof(vec_a) / sizeof(int))
    );
  return 0;
}
