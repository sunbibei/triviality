/*
 * electronics.cpp
 *
 *  Created on: Mar 8, 2018
 *      Author: bibei
 */

#include "electronics.h"

namespace edu {
namespace neu {
namespace csye6205 {

Electronics::Electronics(const std::string& _n, double _p, size_t _num)
  : Item(_n, _p, _num) {
  ;
}

Electronics::~Electronics() {
  ;
}

//std::ostream& operator<<(std::ostream& _os, const Electronics& _item) {
//  char buf[128] = {0};
//  sprintf(buf, "[Electronics](%12s) -> %08.04 / %d", _item.name_.c_str(), _item.price_, _item.number_);
//  _os << buf;
//  return _os;
//}

} /* namespace csye6205 */
} /** end namespace neu */
} /** end namespace edu */
