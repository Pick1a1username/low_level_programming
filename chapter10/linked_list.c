#include <stdio.h>

#include <malloc.h>

struct ll_node {
  struct ll_node* next_node;
  int value;
};

/* Accept a number, returns a pointer to the new linked list node*/
struct ll_node* list_create(int num) {
  /*
  https://www.programiz.com/c-programming/c-structures-pointers
  */

  struct ll_node *new_node;

  new_node = (struct ll_node*) malloc(sizeof( struct ll_node ));

  new_node->value = num;

  return new_node;
}

/* Accept a number and a pointer to a pointer to the linked list.
 * Prepend the new node with a number to the list.
 */
int list_add_front() {
  return 0;
}

/*
 * Add an element to the end of the list.
 * The signature is the same as list_add_front.
 */
int list_add_back() {
  return 0;
}

/*
 * Get an element by index, or returns 0 if the index is
 * outside the list bounds.
 */
int list_get() {
  return 0;
}

/*
 * Free the memory allocated to all elements of list.
 */
int list_free() {
  return 0;
} 

/*
 * Accept a list and computes its length.
 */
int list_length() {
  return 0;
}

/*
 * Accept a list and an index, returns a pointer to struct list,
 * corresponding to the node at this index.
 * If the index is too big, return NULL.
 */
int list_node_at() {
  return 0;
}

/*
 * Accept a list, returns the sum of elements.
 */
int list_sum() {
  return 0;
}

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
  struct ll_node* new_node;
  new_node = list_create(2);

  printf( "%d\n", new_node->value );
  free( new_node );
  return 0;
}

