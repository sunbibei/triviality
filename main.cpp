/*
 * main.cpp
 *
 *  Created on: Mar 9, 2018
 *      Author: bibei
 */

#include <iostream>
#include "master.h"

int main(int argc, char* argv[]) {
  if (2 != argc) {
    std::cout << "Usage: checker [size]" << std::endl;
    return -1;
  }
  int size = atoi(argv[1]);
  if (0 != size%2) {
    return -1;
  }

  // int size = 12;
  if (nullptr == Master::create_instance(size)) {
    std::cout << "Something is wrong!" << std::endl;
    return -1;
  }

  Master::instance()->initialize();
  Master::instance()->run();

  Master::destroy_instance();
  return 0;
}


