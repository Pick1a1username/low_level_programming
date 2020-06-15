#include <stdio.h>

int prime_number( int number ) {
  int i;

  if ( number == 0 || number == 1) return 0;
  if ( number == 2) return 1;

  for( i = 2; i < number; i++ ) {
    if (number % i == 0) return 0;
  }

  return 1;
}

int main( int argc, char** argv ) {
  int number;

  printf("Enter a number: ");
  scanf("%d", &number);

  if (prime_number(number)) {
    printf("It's a prime number!\n");
  } else {
    printf("It isn't a prime number!\n");
  }

  return 0;
}
