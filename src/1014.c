#include <stdio.h>

void input_1014(int* r1, int* r2) {
  scanf("%d %d", r1, r2);
}

const double PI = 3.14;
void handle_1014(int r1, int r2) {
  double areas = PI*(double)r1*(double)r1;
  printf("%6.03f\n", areas);

  double lens = 0;
  for (int r = r1; r <= r2; ++r) {
    lens = 2*PI*(double)r;
    printf("%6.03f\n", lens);
  }
}

// #define TEST1014
#ifdef  TEST1014

int main() {
  // printf("Input: \n");

  int r1 = 0, r2 = 0;
  input_1014(&r1, &r2);

  handle_1014(r1, r2);

  return 0;
}

#endif
