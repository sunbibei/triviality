/*
 * utf.h
 *
 *  Created on: Mar 15, 2018
 *      Author: bibei
 */

#ifndef INCLUDE_UTF_H_
#define INCLUDE_UTF_H_

enum TokenType {
  TK_RED = 0,
  TK_WHITE,
  N_TK_TYPE
};

enum CellType {
  CL_BLACK = 0,
  CL_WHITE,
  N_CL_TYPE
};

///! Anti-Clock
///! 0 X 1
/*!
 * Anti-Clock
 *     0  X  1
 *     X  Me X
 *     3  X  2
 */
enum DiagCell {
  DC_0 = 0,
  DC_1,
  DC_2,
  DC_3,
  N_DC,
};



#endif /* INCLUDE_UTF_H_ */
