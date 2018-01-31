/*
 * main.cpp
 *
 *  Created on: Jan 31, 2018
 *      Author: bibei
 */


#include <iostream>
#include "my_malloc.h"

#define _ROUND_UP1(__bytes, size) \
    (((__bytes) + (size_t)size - 1) & ~((size_t)size - 1))

int main() {
  std::cout << "_ROUND_UP1(20, 1):  " << _ROUND_UP1(20, 1) << std::endl;
  std::cout << "_ROUND_UP1(20, 4):  " << _ROUND_UP1(20, 4) << std::endl;
  std::cout << "_ROUND_UP1(20, 8):  " << _ROUND_UP1(20, 8) << std::endl;
  std::cout << "_ROUND_UP1(20, 9):  " << _ROUND_UP1(20, 9) << std::endl;
  std::cout << "_ROUND_UP1(20, 16): " << _ROUND_UP1(20, 16) << std::endl;
  std::cout << "_ROUND_UP1(20, 20): " << _ROUND_UP1(20, 20) << std::endl;
  std::cout << "_ROUND_UP1(20, 24): " << _ROUND_UP1(20, 24) << std::endl;
  std::cout << "_ROUND_UP1(20, 30): " << _ROUND_UP1(20, 30) << std::endl;
  std::cout << "_ROUND_UP1(20, 40): " << _ROUND_UP1(20, 40) << std::endl;
}
