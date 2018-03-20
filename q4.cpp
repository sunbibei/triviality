/*
 * q4.cpp
 *
 *  Created on: Mar 20, 2018
 *      Author: bibei
 */

// #define Q4_TEST
#ifdef  Q4_TEST

#include <iostream>

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

  int idx = 0;
  long prod = d_buf[idx];
  for (idx = idx + 1; idx < size; ++idx)
    prod *= d_buf[idx];

  if (0 == prod) {
    std::cout << "The number of trailing zeros is 0" << std::endl;

    delete[] d_buf;
    return 0;
  }

  int res = 0;
  while (0 == prod % 10) {
    prod /= 10;
    ++res;
  }

  std::cout << "The number of trailing zeros is " << res << std::endl;

  delete[] d_buf;
  return 0;
}

#endif


