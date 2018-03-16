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
#define TK2STR(t) \
    ( ( (const char*[]){"R E D", "WHITE", "N_TK_TYPE"} )[(t)] )

#define TKOPPONENT(t) \
    ( ( (const TokenType[]){TK_WHITE, TK_RED, N_TK_TYPE} )[(t)] )

enum CellType {
  CL_BLACK = 0,
  CL_WHITE,
  N_CL_TYPE
};
#define CT2STR(t) \
    ( ( (const char*[]){"BLACK", "WHITE", "N_CL_TYPE"} )[t] )

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
#define DC2STR(t) \
    ( ( (const char*[]){"HL", "HR", "FR", "FL", "N_DC"} )[t] )


#endif /* INCLUDE_UTF_H_ */
