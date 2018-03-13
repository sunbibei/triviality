#include <stdio.h>
#include <malloc.h>

const char TAB = 0x09;
const char NLI = 0x0A;
const char REN = 0x0D;
const char SPC = 0x20;

const int  DELI_SIZE = 4;
const char DELI[] = {0x09, 0x0A, 0x0D, 0x20};

int* input_1015(int* size) {
  int* marks = (int*)  malloc(1024 * sizeof(int));
  char* buf  = (char*) malloc(1024 * sizeof(char));
  char* int_buf = (char*) malloc(8 * sizeof(char));
  memset(marks,   0x00, 1024 * sizeof(int));
  memset(buf,     0x00, 1024 * sizeof(char));
  memset(int_buf, 0x00, 8 * sizeof(char));

  *size = 0;
  char c = 0;
  do {
    c = getchar();
    if (EOF == c) break;
    buf[*size] = c;
    ++(*size);
  } while (1);

//  printf("got:\n%s\n", buf);
//  printf("parse int\n");

  char* h = buf;
  char* p = buf;
  char* END = buf + *size;
  *size = 0;
  while (p != END) {
    for (int i = 0; i < DELI_SIZE; ++i) {
      if (DELI[i] == *p) {
        sscanf(h, "%d", marks + *size);
        ++(*size);
        h = p;

        // printf("%d ", marks[*size - 1]);
        break;
      }
    }
    ++p;
  }
  // printf("size: %d\n", *size);

  free(buf);
  return marks;
}

void handle_1015(const int* integers, int size) {
  if ((NULL == integers) || (size <= 0)) return;

  char c = '\0';
  for (int i = 0; i < size; ++i) {
    c = integers[i];
    printf("%c", c);
  }
  printf("\n");
}

// #define TEST1015
#ifdef  TEST1015

int main() {
  // printf("Input: \n");

  int size = 0;
  int* marks = input_1015(&size);

  handle_1015(marks, size);

  free(marks);
  return 0;
}

#endif
