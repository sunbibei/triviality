/*
 * token.h
 *
 *  Created on: Mar 14, 2018
 *      Author: bibei
 */

#ifndef INCLUDE_TOKEN_H_
#define INCLUDE_TOKEN_H_

#include "utf.h"

class Token {
public:
  Token(TokenType, int, class Player*);
  virtual ~Token();

public:
  int       index()  const { return idx_; }
  bool      isKing() const { return is_king_; }
  TokenType type()   const { return type_; }
  bool      canMove(DiagCell) const;

  void move(class Cell* _cell);

  class Cell*   getCell() { return location_; }
  class Player* getPlayer();

private:
  const TokenType    type_;
  const int          idx_;
  class Cell*        location_;
  class Player*      player_;

  bool               is_king_;
};

#endif /* INCLUDE_TOKEN_H_ */
