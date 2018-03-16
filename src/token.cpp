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
  if (!is_king_ && (TokenType::TK_RED == type_)
      && ((DiagCell::DC_2 != dc) && (DiagCell::DC_3 != dc)) ) {
    return false;
  }
  if (!is_king_ && (TokenType::TK_WHITE == type_)
      && ((DiagCell::DC_0 != dc) && (DiagCell::DC_1 != dc)) ) {
    return false;
  }
  ///! No such diagonally cell.
  auto next_cell = location_->diag(dc);
  if (nullptr == next_cell)          return false;
  if (nullptr == next_cell->token()) return true;

  ///! The cell has a piece and the same type.
  if (type_ == next_cell->token()->type()) return false;
  ///! it should be captured.
  auto nnext_cell = next_cell->diag(dc);
  ///! No such d-diagonally cell or has a piece.
  if ( (nullptr == nnext_cell) || (nullptr != nnext_cell->token()) )
    return false;

  return true;
}

void Token::move(Cell* _cell) {
  if ((nullptr != _cell) && !is_king_)
    is_king_  = _cell->king_row(type_);

  location_ = _cell;
}

Player* Token::getPlayer() {
  return player_;
}
