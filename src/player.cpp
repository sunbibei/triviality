/*
 * player.cpp
 *
 *  Created on: Mar 15, 2018
 *      Author: bibei
 */

#include <player.h>
#include <iostream>

#include "cell.h"

Player::Player(TokenType _no, int n_token)
  : N_TOKEN(n_token), idx_(_no) {
  tokens_.resize(n_token);
  for (int n = 0; n < n_token; ++n)
    tokens_[n]   = new Token(_no, n, this);
}

Player::~Player() {
  for (auto& token : tokens_) {
    delete token;
    token = nullptr;
  }
}

bool Player::move(int _no, Cell* _cell) {
  if (!__checkToken(_no)) return false;

  tokens_[_no]->move(_cell);
  return true;
}

bool Player::captured(int _no) {
  if (!__checkToken(_no)) return false;

  delete tokens_[_no];
  tokens_[_no] = nullptr;
  return true;
}

Cell* Player::where(int _no) {
  if (!__checkToken(_no)) return nullptr;

  return tokens_[_no]->getCell();
}

const Token* Player::token(int _no) const {
  if (!__checkToken(_no)) return nullptr;

  return tokens_[_no];
}

Token* Player::token(int _no) {
  if (!__checkToken(_no)) return nullptr;

  return tokens_[_no];
}


inline bool Player::__checkToken(int _no) const {
  if ((_no < 0) || (_no >= N_TOKEN)) {
    std::cout << "error token number!" << std::endl;
    return false;
  }
  if (nullptr == tokens_[_no]) {
    std::cout << "The token has captured!" << std::endl;
    return false;
  }

  return true;
}
