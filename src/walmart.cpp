/*
 * walmart.cpp
 *
 *  Created on: Mar 9, 2018
 *      Author: bibei
 */

#include <walmart.h>
#include "electronics.h"
#include "bread.h"
#include "healthy_bread.h"
#include <algorithm>

namespace edu {
namespace neu {
namespace csye6205 {

void Walmart::demo(const std::string& type) {
  Walmart obj("Walmart");
  std::cout << "\nItems for sale at " << obj.getName() << " (sorted by "
      << type << ")" << std::endl;

  if (0 == type.compare("price")) {
    Electronics sharpTv  ("Sharp Flat Screen TV", 899.99, 100);
    Electronics appleIpad("Apple Ipad mini",      499.99, 101);
    Electronics blueRay  ("BlueRay player",       99.99,  102);
    Electronics hpLaptop ("HP Laptop",            399.99, 103);

    obj.addItem(&sharpTv);
    obj.addItem(&appleIpad);
    obj.addItem(&blueRay);
    obj.addItem(&hpLaptop);
  } else if (0 == type.compare("number")) {
    Bread pretzel        ("Pretzel",         0.49, 3);
    Bread bagel          ("Bagel",           0.89, 1);
    Bread frenchBaguette ("French Baguette", 2.59, 2);
    Bread dinnerRoll     ("Dinner Roll",     1.89, 4);

    obj.addItem(&pretzel);
    obj.addItem(&bagel);
    obj.addItem(&frenchBaguette);
    obj.addItem(&dinnerRoll);
  } else if (0 == type.compare("calories")) {
    HealthyBread pretzelHealthy        ("Pretzel",         0.49, 3, 230.0);
    HealthyBread bagelHealthy          ("Bagel",           0.89, 1, 280.0);
    HealthyBread frenchBaguetteHealthy ("French Baguette", 2.59, 2, 150.0);
    HealthyBread dinnerRollHealthy     ("Dinner Roll",     1.89, 4, 80.0);

    obj.addItem(&pretzelHealthy);
    obj.addItem(&bagelHealthy);
    obj.addItem(&frenchBaguetteHealthy);
    obj.addItem(&dinnerRollHealthy);
  }

  obj.sortInventory(type);
  obj.showInventory();
}

Walmart::Walmart(const std::string& _n)
  : Store(_n) {
  ;
}

Walmart::~Walmart() {
  ;
}

inline void Walmart::addItem(HealthyBread* item) {
  healthy_bread_inv_.push_back(item);
}

inline void Walmart::addItem(Item* item) {
  Store::addItem(item);
}

void Walmart::sortInventory(const std::string& type) {
  if (0 == type.compare("calories")) {
    std::sort(healthy_bread_inv_.begin(), healthy_bread_inv_.end(),
        [](const HealthyBread* _i1, const HealthyBread* _i2) {
      return (_i1->calories() >= _i2->calories());
    });
  } else {
    Store::sortInventory(type);
  }
}

void Walmart::showInventory() {
  if (!inventory_.empty())
    Store::showInventory();

  if (!healthy_bread_inv_.empty()) {
    std::cout << this->healthy_bread_inv_.size() << " items in "
        << getName() << " inventory" << std::endl;
    for (const auto& item : healthy_bread_inv_)
      std::cout << *item << std::endl;
  }
}

} /** end namespace csye6205 */
} /** end namespace neu */
} /** end namespace edu */
