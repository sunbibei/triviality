#include <stdio.h>

void input_1030(int* r1, int* r2, int* r3) {
  scanf("%d %d %d", r1, r2, r3);
}

void handle_1030(int a, int b, int n) {
  a %= b;
  double val = (double)a/b;
  char* buf  = (char*) malloc((n + 2) * sizeof(char));
  char* temp_str = (char*) malloc(16 * sizeof(char));
  memset(buf,      0x00, (n + 2) * sizeof(char));
  memset(temp_str, 0x00, 16 * sizeof(char));

  sprintf(temp_str, "%%%d.%df", n+2, n);
  sprintf(buf, temp_str, val);
  printf("%c\n", buf[n+1]);

  free(buf);
  free(temp_str);
}

// #define TEST1030
#ifdef  TEST1030

int main() {
  // printf("Input: \n");

  int r1 = 0, r2 = 0, r3 = 0;
  input_1030(&r1, &r2, &r3);

  handle_1030(r1, r2, r3);

  return 0;
}

#endif
