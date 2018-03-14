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
  Token(TokenType, int);
  virtual ~Token();

public:
  int       index()  const { return idx_; }
  bool      isKing() const { return is_king_; }
  TokenType type()   const { return type_; }

  void move(class Cell* _cell);

  class Cell* getLocation() { return location_; }

private:
  const TokenType    type_;
  const int          idx_;
  class Cell*        location_;

  bool               is_king_;
};

#endif /* INCLUDE_TOKEN_H_ */
