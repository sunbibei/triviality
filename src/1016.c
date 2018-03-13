#include <stdio.h>

void input_1016(int* r1, int* r2) {
  scanf("%d %d", r1, r2);
}

void handle_1016(int a, int b) {
  int num1 = (a >= b) ? a : b;
  int num2 = (a >= b) ? b : a;
  int c    = 0; // the middle variable.

  while ( num2 > 0 ) {
      c    = num1%num2;
      num1 = num2;
      num2 = c;
  }

  int mul = a*b/num1;
  printf("%d %d\n", num1, mul);
}

// #define TEST1016
#ifdef  TEST1016

int main() {
  // printf("Input: \n");

  int r1 = 0, r2 = 0;
  input_1016(&r1, &r2);

  handle_1016(r1, r2);

  return 0;
}

#endif
