/*
 * master.cpp
 *
 *  Created on: Mar 15, 2018
 *      Author: bibei
 */

#include <master.h>
#include "board.h"
#include "player.h"

#include <sstream>
#include <iostream>


#define KEY_CR (0x0A)

#define INPUT_UNTIL_CR(input) \
    input.clear(); \
    do { \
      char c = '\0'; \
      while (KEY_CR != (c = getchar())) { \
        input.push_back(c); \
      } \
    } while (0)

void __get_move_input(std::vector<DiagCell>&);
int __get_piece_input(Player* _p);

Master* Master::inst_ = nullptr;

void Master::initialize() {
  delete board_;
  for (auto& p : players_)
    delete p;
  ///! create the board
  board_ = new Board(board_size_);

  ///! create the players
  int token_size = board_size_/2*N_INIT_TOKEN_ROWS;
  players_[TokenType::TK_RED]   = new Player(TokenType::TK_RED,   token_size);
  players_[TokenType::TK_WHITE] = new Player(TokenType::TK_WHITE, token_size);

  ///! initialize the board
  board_->initToken(players_[TokenType::TK_RED], players_[TokenType::TK_WHITE]);

  ///! everything is OK
  // std::cout << "The checker has initialized!" << std::endl;
  std::cout << "ROUND 0:" << std::endl;
  board_->print();
}

void Master::run() {
  int round = 1;
  Player* _p = players_[TokenType::TK_RED];
  while (8 != round/*is_end()*/) {
    std::cout << "Turn is the " << ((TokenType::TK_RED == _p->type()) ?
            "red" : "white") << " player!" << std::endl;

    bool is_move = false;
    do {
      int tk_idx = __get_piece_input(_p);
      std::vector<DiagCell> moves;
      do {
        __get_move_input(moves);
      } while (!makeMove(_p->type(), tk_idx, moves));
      is_move = !moves.empty();
    } while (!is_move);

    // print the board
    std::cout << "ROUND " << round << ":" << std::endl;
    board_->print();
    // Advance the variables
    ++round;
    _p = players_[(0 == round % 2) ? TokenType::TK_WHITE : TokenType::TK_RED];
  }
}

bool Master::is_end() {
  // TODO
  return false;
}

bool Master::makeMove(TokenType _type, int tk_idx, const std::vector<DiagCell>& steps) {
  Token* token = players_[_type]->token(tk_idx);

  for (const auto& _s : steps) {
    int captured = -1;
    if (!board_->movePiece(token, _s, captured)) return false;
    if (-1 != captured) {
      players_[(TokenType::TK_RED == _type)?TokenType::TK_WHITE : TokenType::TK_RED]
          ->captured(captured);
    }
  }
  return true;
}

Master* Master::create_instance(int SIZE) {
  if (nullptr == inst_) {
    inst_ = new Master(SIZE);
  }
  return inst_;
}

Master* Master::instance() {
  return inst_;
}

void Master::destroy_instance() {
  delete inst_;
  inst_ = nullptr;
}

Master::Master(int SIZE)
  : board_size_(SIZE), board_(nullptr) {
  for (auto& p : players_)
    p = nullptr;

  std::cout << "================================" << std::endl;
  std::cout << "            READY! GO!          " << std::endl;
  std::cout << "================================" << std::endl;
}

Master::~Master() {
  delete board_;
  board_ = nullptr;
  for (auto& p : players_) {
    delete p;
    p = nullptr;
  }

  std::cout << "================================" << std::endl;
  std::cout << "            GAME OVER!          " << std::endl;
  std::cout << "================================" << std::endl;
}

int __get_piece_input(Player* _p) {
  std::cout << "Choice the piece you want to move." << std::endl;

  int idx = -1;
  std::string input;
  while (true) {
    INPUT_UNTIL_CR(input);
    idx = atoi(input.c_str());
    if (nullptr != _p->where(idx))
      break;

    std::cout << "ERROR INPUT! INPUT AGAIN!" << std::endl;
  }

  return idx;
}

void __get_move_input(std::vector<DiagCell>& moves) {
  std::cout << "How to move?" << std::endl;

  bool is_error = false;
  do {
    is_error = false;
    moves.clear();
    std::string input;
    std::stringstream ss;

    INPUT_UNTIL_CR(input);
    ss << input;

    int direction = 0;
    while (ss >> direction) {
      switch (direction) {
      case 7: moves.push_back(DiagCell::DC_0); break;
      case 9: moves.push_back(DiagCell::DC_1); break;
      case 3: moves.push_back(DiagCell::DC_2); break;
      case 1: moves.push_back(DiagCell::DC_3); break;
      default:
        std::cout << "ERROR INPUT! INPUT AGAIN!" << std::endl;
        is_error = true;
      }
    }
  } while (is_error);

}
