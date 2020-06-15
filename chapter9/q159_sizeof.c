#include <stdio.h>

void f() {
  int x = 10;

  printf("%zu\n", sizeof( void ) );
  printf("%zu\n", sizeof( 0 ) );
  printf("%zu\n", sizeof( 'x' ) );
  printf("%zu\n", sizeof( "hello" ) );
  printf("%zu\n", sizeof( x=90 ) );
  printf("%zu\n", sizeof( x=10 ) );
}

int main( void ) {
  f();
  return 0;
}
