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

void __print_capture(Player* _p0, Player* _p1);

Master* Master::inst_ = nullptr;

void Master::initialize() {
  delete board_;
  for (auto& p : players_)
    delete p;
  ///! create the board
  board_ = new Board(board_size_);

  ///! create the players
  int token_size = board_size_*(board_size_ - 2)/4;
  players_[TokenType::TK_RED]   = new Player(TokenType::TK_RED,   token_size);
  players_[TokenType::TK_WHITE] = new Player(TokenType::TK_WHITE, token_size);

  ///! initialize the board
  board_->initToken(players_[TokenType::TK_RED], players_[TokenType::TK_WHITE]);

  ///! everything is OK
  // std::cout << "The checker has initialized!" << std::endl;
//  std::cout << "ROUND 0:" << std::endl;
//  board_->print();
}

void Master::run() {
  int round = 1;
  TokenType win = TokenType::N_TK_TYPE;
  Player* _p    = players_[TokenType::TK_RED];
  while (!is_end(&win)) {
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
    printf("ROUND [%03d]\n", round);
    printf("Turn to [%5s]\n", TK2STR(_p->type()));
    __print_capture(players_[TokenType::TK_RED], players_[TokenType::TK_WHITE]);
    // print the board
    board_->print();

    if (TokenType::TK_RED == _p->type())
      printf("\033[31m");
    bool is_move = false;
    do {
      int tk_idx = __get_piece_input(_p);
      std::vector<DiagCell> moves;
      do {
        __get_move_input(moves);
      } while (!makeMove(_p->type(), tk_idx, moves));
      is_move = !moves.empty();
    } while (!is_move);

    printf("\033[0m");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");

    // Advance the variables
    ++round;
    _p = players_[(0 == round % 2) ? TokenType::TK_WHITE : TokenType::TK_RED];
  }

  // print the board
  board_->print();
  printf("\033[1;36m  %s WIN!\033[0m\n", TK2STR(win));
}

bool Master::is_end(TokenType* win) {
  for (const auto& _p : players_) {
    ///! the default value.
    if (nullptr != win) *win = TKOPPONENT(_p->type());
    if (0 == _p->n_token()) return true;

    for (int i = _p->N_token() - 1; i >= 0; --i) {
      auto token = _p->token(i);
      if (nullptr == token) continue;
      for (const auto& dc : {DiagCell::DC_0, DiagCell::DC_1, DiagCell::DC_2, DiagCell::DC_3}) {
        if (token->canMove(dc)) return false;
      }
    }
  }

  return true;
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
  std::cout << "Which piece? ";

  int idx = -1;
  std::string input;
  while (true) {
    INPUT_UNTIL_CR(input);
    idx = atoi(input.c_str());
    if ( (0 != idx) || ('0' == input.back()) ) {
      if (nullptr != _p->where(idx))
        break;
    }

    std::cout << "ERROR INPUT[ ";
    for (int i = 0; i < _p->N_token(); ++i)
      if (nullptr != _p->token(i)) std::cout << i << " ";
    std::cout << "]! INPUT AGAIN! ";
  }

  return idx;
}

void __get_move_input(std::vector<DiagCell>& moves) {
  std::cout << "How to move? ";

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
        std::cout << "ERROR INPUT[7, 9, 3, 1]! INPUT AGAIN! ";
        is_error = true;
      }
    }
  } while (is_error);

}

void __print_capture(Player* _p0, Player* _p1) {
  printf("Player: [%s]'s token %02d/%02d -- ", TK2STR(_p0->type()),
      _p0->n_token(), _p0->N_token());

  for (int i = 0; i < _p0->N_token(); ++i)
    if (nullptr == _p0->token(i)) printf("%02d ", i);
  std::cout << std::endl;

  printf("Player: [%s]'s token %02d/%02d -- ", TK2STR(_p1->type()),
      _p1->n_token(), _p1->N_token());

  for (int i = 0; i < _p1->N_token(); ++i)
    if (nullptr == _p1->token(i)) printf("%02d ", i);

  std::cout << std::endl;
}
