#include <stdio.h>
#include <memory.h>
#include <malloc.h>

///!
const char IND_VEHI = '1';
const char IND_NONE = '.';

const int FALSE = 0x00;
const int TRUE  = 0x01;

void input_1084(int*** arrs, const int LANE, const int N_STEP) {
  *arrs = (int**) malloc((N_STEP + 1) * sizeof(int*));
  for (int i = 0; i <= N_STEP; ++i) {
    (*arrs)[i] = (int*) malloc(LANE * sizeof(int));
    memset((*arrs)[i], FALSE, LANE * sizeof(int));
  }

  char buf[16] = {0};
  char c  = 0;
  int idx = 0;
  int tp = 0, tl = 0;
  do {
    c = getchar();
    if (EOF == c) {
      return;
    } else if ('\n' == c) {
      if (0 == idx) {
        continue;
      } else {
        sscanf(buf, "%d %d", &tp, &tl);
        if (-1 == tp && -1 == tl) return;
        if (tp+1 < N_STEP)
          (*arrs)[tp+1][tl] = TRUE;

        idx = 0;
      }
    } else {
      buf[idx++] = c;
    }
  } while (TRUE);
}

void movement_phase(char** highways, const int LANE, const int LEN) {
  for (int l = 0; l < LANE; ++l) {
    char* plane = highways[l];
    plane[LEN - 1] = IND_NONE;
    for (int c = LEN - 1; c > 0; --c) {
      if (IND_VEHI == plane[c - 1]) {
        plane[c]     = IND_VEHI;
        plane[c - 1] = IND_NONE;
      }
    }
  }
}

void arrival_phase(char** highways, int lane) {
  highways[lane][0] = IND_VEHI;
}

///! print the result at time-step s
#define SHOW_HIGHWAY \
    for (int r = 0; r < lane; ++r) { \
      char* rows = highways[r]; \
      for (int c = 0; c < len; ++c) { \
        printf("%c", rows[c]); \
      } \
      printf("\n"); \
    }

#define TEST1084
#ifdef  TEST1084

int main() {
  char** highways = NULL;
  int lane = 0, len = 0, n_step = 0;
  scanf("%d %d %d", &lane, &len, &n_step);

  ///! Create the highway
  highways = (char**) malloc(lane * sizeof(char*));
  for (int l = 0; l < lane; ++l) {
    highways[l] = (char*) malloc(len * sizeof(char));
    memset(highways[l], IND_NONE, len * sizeof(char));
  }

  int** arrival = NULL;
  ///! Got all of the input
  input_1084(&arrival, lane, n_step);

  for (int s = 0; s <= n_step; ++s) {
    movement_phase(highways, lane, len);
    for (int l = 0; l < lane; ++l) {
      if (TRUE == arrival[s][l])
        arrival_phase(highways, l);
    }
//    printf("t=%d\n", s);
//    SHOW_HIGHWAY
  }

  SHOW_HIGHWAY

  ///! free highways
  for (int h = 0; h < lane; ++h) {
    free(highways[h]);
  }
  free(highways);
  ///! free arrival
  for (int i = 0; i < n_step; ++i) {
    free(arrival[i]);
  }
  free(arrival);

  return 0;
}

#endif
