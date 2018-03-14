/*
 * master.h
 *
 *  Created on: Mar 15, 2018
 *      Author: bibei
 */

#ifndef INCLUDE_MASTER_H_
#define INCLUDE_MASTER_H_

#include "utf.h"

class Master {
///! singleton
public:
  static Master* create_instance(int);
  static Master* instance();
  static void    destroy_instance();

public:
  void initialize();
  void run();

protected:
  Master(int);
  virtual ~Master();

  static Master* inst_;

protected:
  int           board_size_;
  class Board*  board_;
  class Player* players_[TokenType::N_TK_TYPE];

};

#endif /* INCLUDE_MASTER_H_ */
