/*
 * q6.cpp
 *
 *  Created on: Mar 20, 2018
 *      Author: bibei
 */

#define Q6_TEST
#ifdef  Q6_TEST

#include <iostream>

bool __check_reach_last(int*, int);

int main(int argc, char* argv[]) {
  int MIN_SIZE = 4;
  int MAX_SIZE = 10;
  int size = 0;

  std::cout << "Please input the number of non-negative integers:" << std::endl;
  std::cin  >> size;
  if ( (size < MIN_SIZE) || (size >= MAX_SIZE) ) {
    std::cout << "ERROR INPUT!" << std::endl;
    return -1;
  }

  std::cout << "Now input the sequence of non-negative integers:" << std::endl;
  int* d_buf = new int[size];
  for (int r = size - 1; r >= 0; --r) {
    std::cin >> d_buf[r];
    if (d_buf[r] < 0) {
      std::cout << "ERROR INPUT!" << std::endl;

      delete[] d_buf;
      return -1;
    }
  }

  std::cout << "The reversed sequence of non-negative intgers is:" << std::endl;
  for (int r = 0; r < size; ++r)
    printf("%2d ", d_buf[r]);
  printf("\n");

  std::cout << "You " << (__check_reach_last(d_buf, size) ? "can" : "cannot")
      << " reach the last index." << std::endl;

//  for (int i = 0; i < size; ++i) {
//    for (int ii = d_buf[i]; ii >= 0; --ii) {
//      if (ii >= size) return true;
//    }
//  }

  delete[] d_buf;
  return 0;
}

bool __check_reach_last(int* seqs, int size) {
  if ((0 == size) || (seqs[0] > size)) return true;

  for (int i = seqs[0]; i > 0; --i)
    if (__check_reach_last(seqs + i, size - i)) return true;

  return false;
}

#endif


