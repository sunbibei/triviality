/*
 * item.cpp
 *
 *  Created on: Mar 8, 2018
 *      Author: bibei
 */

#include <item.h>
#include <typeinfo>

namespace edu {
namespace neu {
namespace csye6205 {

Item::Item(const std::string& _n, double _p, size_t _num)
  : name_(_n), price_(_p), number_(_num) {
  ;
}

Item::~Item() {
  ;
}

std::string& Item::name()   { return name_;   }
double&      Item::price()  { return price_;  }
size_t&      Item::number() { return number_; }

const std::string& Item::name()   const { return name_;   }
const double&      Item::price()  const { return price_;  }
const size_t&      Item::number() const { return number_; }

std::ostream& operator<<(std::ostream& _os, const Item& _item) {
  char buf[128] = {0};
  sprintf(buf, "[%s](%24s)\t %08.04f / %04ld", typeid(_item).name(), _item.name_.c_str(), _item.price_, _item.number_);
  _os << buf;
  return _os;
}

} /* namespace csye6205 */
} /** end namespace neu */
} /** end namespace edu */
