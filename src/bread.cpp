/*
 * bread.cpp
 *
 *  Created on: Mar 8, 2018
 *      Author: bibei
 */

#include <bread.h>

namespace edu {
namespace neu {
namespace csye6205 {

Bread::Bread(const std::string& _n, double _p, size_t _num)
  : Item(_n, _p, _num) {
  ;
}

Bread::~Bread() {
  ;
}

//std::ostream& operator<<(std::ostream& _os, const Bread& _item) {
//  char buf[128] = {0};
//  sprintf(buf, "[Item](%12s) -> %08.04 / %d", _item.name_.c_str(), _item.price_, _item.number_);
//  _os << buf;
//  return _os;
//}

} /* namespace csye6205 */
} /** end namespace neu */
} /** end namespace edu */
