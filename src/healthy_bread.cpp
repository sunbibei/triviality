/*
 * healthy_bread.cpp
 *
 *  Created on: Mar 8, 2018
 *      Author: bibei
 */

#include <healthy_bread.h>

namespace edu {
namespace neu {
namespace csye6205 {

HealthyBread::HealthyBread(const std::string& _n, double _p, size_t _num, double _c)
  : Item(_n, _p, _num), calories_(_c) {
  ;
}

HealthyBread::~HealthyBread() {
  ;
}

std::ostream& operator<<(std::ostream& _os, const HealthyBread& _item) {
  char buf[128] = {0};
  sprintf(buf, "[%s](%s) -> %08.04f / %04ld", typeid(_item).name(), _item.name_.c_str(), _item.price_, _item.number_);
  _os << buf;
  return _os;
}

} /* namespace csye6205 */
} /** end namespace neu */
} /** end namespace edu */
