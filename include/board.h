/*
 * board.h
 *
 *  Created on: Mar 14, 2018
 *      Author: bibei
 */

#ifndef INCLUDE_BOARD_H_
#define INCLUDE_BOARD_H_

#include <vector>

#include "cell.h"
#include "token.h"

/*!
 * The board class, the coordinate system as follow:
 * o -> +y
 *   0 1 2 3 4 5 6 7
 * | 1
 * v 2
 *   3
 * + 4
 * x 5
 *   6
 *   7
 */
class Board {
public:
  ///! Only initialize the board, no token is placed.
  Board(int SIZE);
  virtual ~Board();

public:
  void initToken(class Player* _p0, class Player* _p1);

  void availableDiag(std::vector<const Cell*>&);
  /*!
   * @brief Move the token @_token to the cell @_cell, the index of token @captured
   *        is captured, the @captured is -1, if these are not token is captured.
   */
  bool movePiece(Token* _token, DiagCell _dc, int& captured);

  // bool canMove(Token* _token, std::vector<DiagCell>& _dc);

  ///! printf the board
  void print();
private:
  int SIZE;
  std::vector<std::vector<Cell*>> cells_;
  std::vector<std::vector<Cell*>> black_cells_;
};

#endif /* INCLUDE_BOARD_H_ */
