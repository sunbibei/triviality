/*
 * abstract_store.h
 *
 *  Created on: Mar 8, 2018
 *      Author: bibei
 */

#ifndef INCLUDE_ABSTRACT_STORE_H_
#define INCLUDE_ABSTRACT_STORE_H_

#include <vector>
#include "item.h"

namespace edu {
namespace neu {
namespace csye6205 {

class AbstractStore {
public:
  AbstractStore()          { };
  virtual ~AbstractStore() { };

public:
  virtual void addItem(Item*)  = 0;
  virtual void sortInventory() = 0;
  virtual void showInventory() = 0;

protected:
  std::vector<Item*> inventory_;
};

} /** end namespace csye6205 */
} /** end namespace neu */
} /** end namespace edu */

#endif /* INCLUDE_ABSTRACT_STORE_H_ */
