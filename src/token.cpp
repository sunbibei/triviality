/*
 * token.cpp
 *
 *  Created on: Mar 15, 2018
 *      Author: bibei
 */

#include "token.h"
#include "cell.h"
#include "player.h"

Token::Token(TokenType type, int num, Player* _p)
  : type_(type), idx_(num), location_(nullptr),
    player_(_p), is_king_(false) {
  ;
}

Token::~Token() {
  ;
}

bool Token::canMove(DiagCell dc) const {
  if (is_king_) return true;

  if (TokenType::TK_RED == type_) {
    return ( (DiagCell::DC_2 == dc) || (DiagCell::DC_3 == dc) );
  } else {
    return ( (DiagCell::DC_0 == dc) || (DiagCell::DC_1 == dc) );
  }
}

void Token::move(Cell* _cell) {
  location_ = _cell;
}

Player* Token::getPlayer() {
  return player_;
}
