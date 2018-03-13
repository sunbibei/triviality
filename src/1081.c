#include <stdio.h>
#include <memory.h>
#include <malloc.h>

///! Current Death, Next Death
const char CDND = 0x00;
///! Current Live,  Next Death
const char CLND = 0x01;
///! Current Death, Next Live
const char CDNL = 0x02;
///! Current Live,  Next Live
const char CLNL = 0x03;

///!
const char IND_LIVE  = 'X';
const char IND_DEATH = '.';

void input_1081(char*** board, int* width, int* height, int* n_step) {
  scanf("%d %d %d", height, width, n_step);

  *board = (char **) malloc(*height * sizeof(char*));
  for (int h = 0; h < *height; ++h) {
    (*board)[h] = (char*) malloc(*width * sizeof(char));
    char* rows = (*board)[h];

    for (int w = 0; w < *width; ++w) {
      do {
        scanf("%c", rows + w);
      } while ( (IND_LIVE != rows[w]) && (IND_DEATH != rows[w]) );

      if (IND_LIVE == rows[w])
        rows[w] = CLND;
      else
        rows[w] = CDND;
    }
  }
}

int __max(int a, int b) { return (a > b) ? a : b; }
int __min(int a, int b) { return (a > b) ? b : a; }

int getlives(char** board, const int WIDTH, const int HEIGHT, int i, int j) {
  int res = 0;
  for (int h = __max(i-1, 0); h <= __min(i+1, HEIGHT - 1); ++h) {
    for (int w = __max(j-1, 0); w <= __min(j+1, WIDTH - 1); ++w) {
      res += board[h][w] & 0x01;
    }
  }

  // except it-self
  res -= board[i][j] & 0x01;
  return res;
}

void handle_1081(char** board, const int WIDTH, const int HEIGHT, const int N_STEP) {
  if ((NULL == board) || (WIDTH <= 0) || (HEIGHT <= 0) || (N_STEP <= 0)) return;

  for (int s = 0; s < N_STEP; ++s) {
    for (int r = 0; r < HEIGHT; ++r) {
      char* rows = board[r];
      for (int c = 0; c < WIDTH; ++c) {
        int l_num = getlives(board, WIDTH, HEIGHT, r, c);
        if ((CLND == rows[c]) && ((2 == l_num) || (3 == l_num))) {
          rows[c] = CLNL;
        } else if ((CDND == rows[c]) && (3 == l_num)) {
          rows[c] = CDNL;
        } else {
          ;
        }
      }
    }

    for (int r = 0; r < HEIGHT; ++r) {
      char* rows = board[r];
      for (int c = 0; c < WIDTH; ++c) {
        rows[c] >>= 1;
      }
    }

  } // end N_STEP

  ///! print the result.
  for (int r = 0; r < HEIGHT; ++r) {
    char* rows = board[r];
    for (int c = 0; c < WIDTH; ++c) {
      if (CLND == rows[c]) {
        printf("%c", IND_LIVE);
      } else {
        printf("%c", IND_DEATH);
      }
    }
    printf("\n");
  }
}

#define TEST1081
#ifdef  TEST1081

int main() {
  // printf("Input: \n");

  char** board = NULL;
  int width = 0, height = 0, n_step = 0;
  input_1081(&board, &width, &height, &n_step);

//  printf("\nThe Board:\n");
//  for (int r = 0; r < height; ++r) {
//    char* rows = board[r];
//    for (int c = 0; c < width; ++c) {
//      if (CLND == rows[c]) {
//        printf("%c", IND_LIVE);
//      } else {
//        printf("%c", IND_DEATH);
//      }
//    }
//    printf("\n");
//  }

  handle_1081(board, width, height, n_step);
  for (int h = 0; h < height; ++h) {
    free(board[h]);
  }
  free(board);

  return 0;
}

#endif
