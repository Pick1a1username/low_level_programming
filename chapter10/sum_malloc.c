#include <stdio.h>

#include <malloc.h>

int* array_read( size_t* out_count ) {
  int* array;
  size_t i;
  size_t cnt;

  printf("Enter the number of elements: ");
  scanf( "%zu", &cnt );
  array = malloc( cnt * sizeof( int ) );

  for( i = 0; i < cnt; i++ ) {
    printf("Enter the value of %zu-th element: ", i);
    scanf( "%d", &array[i] );
  }

  *out_count = cnt;
  return array;
}

void array_print( int const* array, size_t count ) {
  size_t i;

  for( i = 0; i < count; i++ ) printf( "%d ", array[i] );

  puts("");
}

int array_sum( int const* array, size_t count ) {
  size_t i;
  int sum = 0;

  for( i = 0; i < count; i++ ) sum = sum + array[i];

  return sum;
}

int main( void ) {
  int* array;
  size_t count;
  array = array_read( &count );
  array_print( array, count );

  printf( "Sum is: %d\n", array_sum( array, count ) );
  free( array );
  return 0;
}

