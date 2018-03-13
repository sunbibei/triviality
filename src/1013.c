#include <stdio.h>
#include <malloc.h>

int* input_1013(int* size) {
  int* marks = (int*) malloc(1024 * sizeof(int));
  memset(marks, 0x00, 1024 * sizeof(int));
  *size = 0;
  int tmp = 0;
  scanf("%d", &tmp);

  while (0 != tmp) {
    marks[*size] = tmp;
    ++(*size);

    scanf("%d", &tmp);
  }

  return marks;
}

void handle_1013(const int* marks, int size) {
  if ((NULL == marks) || (size <= 0)) return;

  int ge_num = 0;
  int be_num = 0;
  int lt_num = 0;
  for (int i = 0; i < size; ++i) {
    if (marks[i] >= 85)
      ++ge_num;
    else if ( (marks[i] >= 60) && (marks[i] <= 84) )
      ++be_num;
    else // < 60
      ++lt_num;
  }
  printf(">=85:%d\n",  ge_num);
  printf("60-84:%d\n", be_num);
  printf("<60:%d\n",   lt_num);
}

// #define TEST1013
#ifdef  TEST1013

int main() {
  // printf("Input: \n");

  int size = 0;
  int* marks = input_1013(&size);

  handle_1013(marks, size);

  free(marks);

  return 0;
}

#endif
