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
  Cell(int _x, int _y, CellType _t, bool _king_row)
  : x(_x), y(_y), type(_t), is_king_row(_king_row),
    token(nullptr) {
    for (auto& dc : diags)
      dc = nullptr;
  }

private:
  const int      x;
  const int      y;
  const CellType type;
  const bool     is_king_row;

  Cell*          diags[N_DC];

  const Token*   token;
};

#endif /* INCLUDE_CELL_H_ */
