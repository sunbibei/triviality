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

  for (int x = 0; x < SIZE; ++x) {
    auto& rows = cells_[x];
    TokenType t = TokenType::N_TK_TYPE;
    if (0 == x) t = TokenType::TK_WHITE;
    if ((SIZE - 1) == x) t = TokenType::TK_RED;

    for (int y = 0; y < SIZE; ++y) {
      CellType type = ( ((0 == x%2) && (0 == y%2)) || ((1 == x%2) && (1 == y%2)) )
          ? CellType::CL_BLACK : CellType::CL_WHITE;

      rows[y] = new Cell(x, y, type, t);
    }
  }

  for (int x = 0; x < SIZE; ++x) {
    auto& b_rows = black_cells_[x];
    for (int y = 0; y < SIZE; ++y) {
      if (CellType::CL_BLACK != cells_[x][y]->type) continue;

      // find the lh
      if ( ((x - 1) >= 0) && ((y - 1) >= 0) )
        cells_[x][y]->diags_[DiagCell::DC_0] = cells_[x-1][y-1];
      // find the rh
      if ( ((x - 1) >= 0) && ((y + 1) < SIZE) )
        cells_[x][y]->diags_[DiagCell::DC_1] = cells_[x-1][y+1];
      // find the rf
      if ( ((x + 1) < SIZE) && ((y + 1) < SIZE) )
        cells_[x][y]->diags_[DiagCell::DC_2] = cells_[x+1][y+1];
      // find the lf
      if ( ((x + 1) < SIZE) && ((y - 1) >= 0) )
        cells_[x][y]->diags_[DiagCell::DC_3] = cells_[x+1][y-1];

      b_rows.push_back(cells_[x][y]);
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
  Player* red   = (TokenType::TK_RED   == _p0->type()) ? _p0 : _p1;
  Player* white = (TokenType::TK_WHITE == _p0->type()) ? _p0 : _p1;

  int token_idx = 0;
  for (size_t i = 0; i < black_cells_.size(); ++i) {
    auto& rows = black_cells_[i];
    for (auto& cell : rows) {
      red->move(token_idx, cell);
      cell->token_ = red->token(token_idx);
      ++token_idx;
    }
    if (token_idx == red->N_token()) break;
  }

  token_idx = 0;
  for (int i = SIZE - 1; i >= 0; --i) {
    auto& rows = black_cells_[i];
    for (auto& cell : rows) {
      white->move(token_idx, cell);
      cell->token_ = white->token(token_idx);
      ++token_idx;
    }
    if (token_idx == white->N_token()) break;
  }
}

bool Board::movePiece(Token* _token, DiagCell _dc, int& captured) {
  captured = -1; // The default value.

  if (!_token->canMove(_dc)) {
    std::cout << "The piece[" << TK2STR(_token->type()) << "-"<< _token->index()
        << "] can't move to the place: " << DC2STR(_dc) << std::endl;
    return false;
  }

  auto prev_cell = _token->getCell();
  auto next_cell = _token->getCell()->diags_[_dc];

  if (nullptr == next_cell->token()) {
    _token->move(next_cell);
    // update this cell.
    next_cell->token_ = _token;
    prev_cell->token_ = nullptr;
    return true;
  }

  ///! The player want to jump to the next-next cell.
  auto nnext_cell = next_cell->diags_[_dc];
  ///! ready to captured an opponent’s piece
  ///! move
  _token->move(nnext_cell);
  ///! then kill an opponent's piece
  captured = next_cell->token_->index();
  // update this cell.
  nnext_cell->token_ = _token;
  next_cell->token_  = nullptr;
  prev_cell->token_  = nullptr;
  return true;
}

void Board::print() {
  // printf("THE BOARD:\n");
  for (const auto& rows : cells_) {
    for (const auto& cell : rows) {
      if (CellType::CL_BLACK == cell->type) {
        // print the cell
        if (nullptr != cell->token_) {
          if (TokenType::TK_RED == cell->token_->type()) {
            if (cell->token_->isKing())
              printf("\033[1;31;47m%2d\033[0m", cell->token_->index());
            else
              printf("\033[31;47m%2d\033[0m", cell->token_->index());
          } else {
            if (cell->token_->isKing())
              printf("\033[1;30;47m%2d\033[0m", cell->token_->index());
            else
              printf("\033[30;47m%2d\033[0m", cell->token_->index());
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
