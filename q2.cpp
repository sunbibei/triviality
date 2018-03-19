/*
 * q2.cpp
 *
 *  Created on: Mar 19, 2018
 *      Author: bibei
 */
// #define Q2_TEST
#ifdef  Q2_TEST

#include <iostream>

void __print_rec(int**, int, int, int);

int main(int argc, char* argv[]) {
  int M_SIZE = 0;

  std::cout << "Please input the size of the matrix:" << std::endl;
  std::cin >> M_SIZE;
  if (M_SIZE <= 0) {
    std::cout << "ERROR INPUT!" << std::endl;
    return -1;
  }

  std::cout << "Please input the matrix row by row:" << std::endl;
  int** d_buf = new int*[M_SIZE];
  for (int r = 0; r < M_SIZE; ++r) {
    int* rows = new int[M_SIZE];
    for (int c = 0; c < M_SIZE; ++c) {
      std::cin >> rows[c];
    }

    d_buf[r] = rows;
  }

  ///! Just for debug
//  for (int r = 0; r < M_SIZE; ++r) {
//    for (int c = 0; c < M_SIZE; ++c)
//      printf("%2d ", d_buf[r][c]);
//
//    std::cout << std::endl;
//  }

  std::cout << "The spiral form of the matrix is" << std::endl;
  __print_rec(d_buf, 0, 0, M_SIZE);


  for (int r = 0; r < M_SIZE; ++r)
    delete[] d_buf[r];
  delete[] d_buf;

  return 0;
}

void __print_rec(int** mat, int S_R, int S_C, int SIZE) {
  if (0 == SIZE) return;

  int r = S_R;
  int c = S_C;
  // --->
  for (; c < S_C + SIZE; ++c)
    std::cout << mat[r][c] << " ";
  --c;
  // |
  // V
  for (r = r + 1; r < S_R + SIZE; ++r)
    std::cout << mat[r][c] << " ";
  --r;
  // <---
  for (c = c - 1; c >= S_C; --c)
    std::cout << mat[r][c] << " ";
  ++c;
  // ^
  // |
  for (r = r - 1; r > S_R; --r)
    std::cout << mat[r][c] << " ";

  __print_rec(mat, S_R + 1, S_C + 1, SIZE - 2);
}

#endif
