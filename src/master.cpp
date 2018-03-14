/*
 * master.cpp
 *
 *  Created on: Mar 15, 2018
 *      Author: bibei
 */

#include <master.h>
#include "board.h"
#include "player.h"

#include <iostream>

Master* Master::inst_ = nullptr;

void Master::initialize() {
  delete board_;
  for (auto& p : players_)
    delete p;
  ///! create the board
  board_ = new Board(8);

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
  ;
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

