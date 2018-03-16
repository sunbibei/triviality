/*
 * cell.h
 *
 *  Created on: Mar 14, 2018
 *      Author: bibei
 */

#ifndef INCLUDE_CELL_H_
#define INCLUDE_CELL_H_

#include "utf.h"
#include "token.h"


class Cell {
  friend class Board;
public:
  Cell(int _x, int _y, CellType _t,
      TokenType t = TokenType::N_TK_TYPE)
  : x(_x), y(_y), type(_t), token_(nullptr) {
    for (auto& dc : diags_)
      dc = nullptr;
    for (auto& r : is_king_row_)
      r = false;
    if (TokenType::N_TK_TYPE != t)
      is_king_row_[t] = true;
  }

  const bool   king_row(TokenType t) const { return is_king_row_[t]; }
  const Cell*  diag(DiagCell dc)     const { return diags_[dc]; }
  const Token* token()               const { return token_; }

private:
  const int      x;
  const int      y;
  const CellType type;
  bool     is_king_row_[TokenType::N_TK_TYPE];

  Cell*          diags_[N_DC];

  const Token*   token_;
};

#endif /* INCLUDE_CELL_H_ */
