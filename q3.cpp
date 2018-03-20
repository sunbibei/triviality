/*
 * q3.cpp
 *
 *  Created on: Mar 20, 2018
 *      Author: bibei
 */

// #define Q3_TEST
#ifdef  Q3_TEST

#include <iostream>

#define INVALID_INT (0)

int main(int argc, char* argv[]) {
  int MIN_SIZE = 4;
  int MAX_SIZE = 20;
  int size = 0;

  std::cout << "Please input the number of integers:" << std::endl;
  std::cin  >> size;
  if ( (size < MIN_SIZE) || (size >= MAX_SIZE) ) {
    std::cout << "ERROR INPUT!" << std::endl;
    return -1;
  }

  std::cout << "Please input the sequence of integers:" << std::endl;
  int* d_buf = new int[size];
  for (int r = 0; r < size; ++r)
      std::cin >> d_buf[r];
  if (INVALID_INT != d_buf[size - 1]) {
    std::cout << "ERROR INPUT!" << std::endl;

    delete[] d_buf;
    return -1;
  }

  std::cout << "Original sequence: ";
  for (int r = 0; r < size; ++r)
    printf("%2d ", d_buf[r]);
  printf("\n");

  std::cout << "Modified sequence: ";
  // step 1
  for (int i = 0; i < size - 1; ++i) {
    if ( (INVALID_INT != d_buf[i + 1]) &&
        (d_buf[i] == d_buf[i + 1]) ) {
      d_buf[i]  *= 2;
      d_buf[i+1] = 0;
    }
  }

  // step 2
  int count = 0;
  for (int i = 0; i < size; ++i)
    if (INVALID_INT != d_buf[i]) {
      printf("%2d ", d_buf[i]);
      ++count;
    }
  for (int i = count; i < size; ++i)
    printf("%2d ", 0);
  printf("\n");


  delete[] d_buf;
  return 0;
}

#endif



