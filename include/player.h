/*
 * player.h
 *
 *  Created on: Mar 15, 2018
 *      Author: bibei
 */

#ifndef INCLUDE_PLAYER_H_
#define INCLUDE_PLAYER_H_

#include <vector>
#include "token.h"

class Player {
public:
  Player(TokenType, int);
  virtual ~Player();

public:
  TokenType index() const { return idx_; }

  ///! the token handle
  const Token* token(int _no) const;

  ///! move the token which with the given index @_no to the cell @_cell.
  bool move(int _no, class Cell* _cell);

  ///! the token has captured.
  bool captured(int _no);

  ///! where the token.
  class Cell* where(int _no);

private:
  bool __checkToken(int _no) const;

private:
  const int N_TOKEN;
  const TokenType idx_;
  std::vector<Token*> tokens_;
};

#endif /* INCLUDE_PLAYER_H_ */
