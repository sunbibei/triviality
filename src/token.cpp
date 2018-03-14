/*
 * token.cpp
 *
 *  Created on: Mar 15, 2018
 *      Author: bibei
 */

#include "token.h"
#include "cell.h"

Token::Token(TokenType type, int num)
  : type_(type), idx_(num), location_(nullptr) {
  ;
}

Token::~Token() {
  ;
}

void Token::move(Cell* _cell) {
  location_ = _cell;
}
