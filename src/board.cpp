/*
 * board.cpp
 *
 *  Created on: Mar 14, 2018
 *      Author: bibei
 */

#include "board.h"
#include "player.h"

#include <iostream>

///!
Board::Board(int _SIZE)
  : SIZE(_SIZE) {
  cells_.resize(SIZE);
  for (auto& rows : cells_)
    rows.resize(SIZE);

  black_cells_.resize(SIZE);
  for (auto& rows : black_cells_)
    rows.reserve(SIZE/2);

  for (int r = 0; r < SIZE; ++r) {
    auto& rows = cells_[r];
    bool king = ( (0 == r) || ((SIZE - 1) == r) );
    for (int c = 0; c < SIZE; ++c) {
      CellType type = ( ((0 == r%2) && (0 == c%2)) || ((1 == r%2) && (1 == c%2)) )
          ? CellType::CL_BLACK : CellType::CL_WHITE;

      Cell* cell = new Cell(r, c, type, king);
      rows[c] = cell;
    }
  }

  for (int x = 0; x < SIZE; ++x) {
    auto& rows   = cells_[x];
    auto& b_rows = black_cells_[x];
    for (int y = 0; y < SIZE; ++y) {
      if (CellType::CL_BLACK != rows[y]->type) continue;

      // find the lh
      if ( ((y - 1) >= 0) && ((x - 1) >= 0) )
        rows[y]->diags[DiagCell::DC_0] = cells_[x-1][y-1];
      // find the lf
      if ( ((y + 1) < SIZE) && ((x - 1) >= 0) )
        rows[y]->diags[DiagCell::DC_1] = cells_[x-1][y+1];
      // find the rh
      if ( ((y + 1) < SIZE) && ((x + 1) < SIZE) )
        rows[y]->diags[DiagCell::DC_2] = cells_[x+1][y+1];
      // find the rf
      if ( ((y + 1) < SIZE) && ((x - 1) >= 0) )
        rows[y]->diags[DiagCell::DC_3] = cells_[x-1][y+1];

      b_rows.push_back(rows[y]);
    }
  }
}

Board::~Board() {
  for (auto& rows : cells_) {
    for (auto& cell : rows) {
      delete cell;
      cell = nullptr;
    }
  }
}

void Board::initToken(Player* _p0, Player* _p1) {
  int token_idx = 0;
  for (int i = 0; i < N_INIT_TOKEN_ROWS; ++i) {
    auto& rows = black_cells_[i];
    for (auto& cell : rows) {
      _p0->move(token_idx, cell);
      cell->token = _p0->token(token_idx);
      ++token_idx;
    }
  }

  token_idx = 0;
  for (int i = SIZE - 1; i >= (SIZE - N_INIT_TOKEN_ROWS); --i) {
    auto& rows = black_cells_[i];
    for (auto& cell : rows) {
      _p1->move(token_idx, cell);
      cell->token = _p1->token(token_idx);
      ++token_idx;
    }
  }
}

void Board::movePiece(Token* _token, Cell* _cell) {
  auto prev_cell   = _token->getLocation();
  if (nullptr != prev_cell->token)
    prev_cell->token = nullptr;

  _token->move(_cell);
}

void Board::print() {
  // printf("THE BOARD:\n");
  for (const auto& rows : cells_) {
    for (const auto& cell : rows) {
      if (CellType::CL_BLACK == cell->type) {
        // print the cell
        if (nullptr != cell->token) {
          if (TokenType::TK_RED == cell->token->type()) {
            if (cell->token->isKing())
              printf("\033[1;31;47m%2d\033[0m", cell->token->index());
            else
              printf("\033[31;47m%2d\033[0m", cell->token->index());
          } else {
            if (cell->token->isKing())
              printf("\033[1;30;47m%2d\033[0m", cell->token->index());
            else
              printf("\033[1;30;47m%2d\033[0m", cell->token->index());
          }
        } else {
          printf("\033[30;47m  \033[0m");
        }
      } else {
        printf("  ");
      }
    }
    printf("\n");
  }
}
