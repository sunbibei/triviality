/*
 * store.cpp
 *
 *  Created on: Mar 9, 2018
 *      Author: bibei
 */

#include <store.h>
#include <iostream>
#include <algorithm>

namespace edu {
namespace neu {
namespace csye6205 {

Store::Store(const std::string& _n)
  : name_(_n) {
  ;
}

Store::~Store() {
  ;
}

inline const std::string& Store::getName() {
  return name_;
}

inline void Store::addItem(Item* _item) {
  inventory_.push_back(_item);
}

void Store::sortInventory(const std::string& type) {
  if (0 == type.compare("price")) {
    std::sort(inventory_.begin(), inventory_.end(),
        [](const Item* _i1, const Item* _i2) {
      return (_i1->price() >= _i2->price());
    });

  } else if (0 == type.compare("number")) {
    std::sort(inventory_.begin(), inventory_.end(),
        [](const Item* _i1, const Item* _i2) {
      return _i1->number() >= _i2->number();
    });

  } else {
    std::cout << "The error type for sorting inventory.";
  }

}

void Store::showInventory() {
  std::cout << inventory_.size() << " items in "
      << getName() << " inventory" << std::endl;
  for (const auto& item : inventory_)
    std::cout << *item << std::endl;
}

} /* namespace csye6205 */
} /** end namespace neu */
} /** end namespace edu */
