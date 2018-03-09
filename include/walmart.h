/*
 * walmart.h
 *
 *  Created on: Mar 9, 2018
 *      Author: bibei
 */

#ifndef INCLUDE_WALMART_H_
#define INCLUDE_WALMART_H_

#include "store.h"
#include "healthy_bread.h"

namespace edu {
namespace neu {
namespace csye6205 {

class Walmart : public Store {
public:
  Walmart(const std::string&);
  virtual ~Walmart();

///! Inherit from AbstractStore
public:
  virtual void sortInventory(const std::string&) override;
  virtual void showInventory() override;

///! Override the addItem.
public:
  virtual void addItem(HealthyBread* item);
  virtual void addItem(Item* item);

public:
  static void demo(const std::string& type);

protected:
  std::vector<HealthyBread*> healthy_bread_inv_;
};

} /** end namespace csye6205 */
} /** end namespace neu */
} /** end namespace edu */

#endif /* INCLUDE_WALMART_H_ */
