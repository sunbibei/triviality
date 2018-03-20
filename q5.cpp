/*
 * q3.cpp
 *
 *  Created on: Mar 20, 2018
 *      Author: bibei
 */
// #define Q5_TEST
#ifdef  Q5_TEST

#include <iostream>

bool is_toepliz(int**, int);

int main(int argc, char* argv[]) {
  int M_SIZE = 0;

  std::cout << "Please input the size of the matrix:" << std::endl;
  std::cin >> M_SIZE;
  if ( (M_SIZE < 3) || (M_SIZE > 10) ) {
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


  std::cout << "Matrix is" << (is_toepliz(d_buf, M_SIZE) ? " " : " not ")
      << "a Toepliz" << std::endl;

  for (int r = 0; r < M_SIZE; ++r)
    delete[] d_buf[r];
  delete[] d_buf;

  return 0;
}

bool is_toepliz(int** mat, int SIZE) {
  // upper triangle
  int r = 0; int c = 0;
  for (c = 0; c < SIZE; ++c) {
    int r1 = r; int c1 = c;
    while (((r1 + 1) < SIZE) && ((c1 + 1) < SIZE )) {
      if (mat[r1][c1] != mat[r1 + 1][c1 + 1]) return false;

      ++r1; ++c1;
    }
  }
  // lower triangle
  c = 0;
  for (r = 1; r < SIZE; ++r) {
    int r1 = r; int c1 = c;
    while (((r1 + 1) < SIZE) && ((c1 + 1) < SIZE )) {
      if (mat[r1][c1] != mat[r1 + 1][c1 + 1]) return false;

      ++r1; ++c1;
    }
  }

  return true;
}

#endif



