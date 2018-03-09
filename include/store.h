/*
 * store.h
 *
 *  Created on: Mar 9, 2018
 *      Author: bibei
 */

#ifndef INCLUDE_STORE_H_
#define INCLUDE_STORE_H_

#include "abstract_store.h"
#include <string>

namespace edu {
namespace neu {
namespace csye6205 {

class Store : public AbstractStore {
public:
  Store(const std::string&);
  virtual ~Store();

///! inherit from AbstractStore
public:
  virtual void addItem(Item*)  override;
  virtual void showInventory() override;

  virtual void sortInventory(const std::string&) override;

public:
  const std::string& getName();

protected:
  std::string        name_;
  std::vector<Item*> inventory_;
};


} /* namespace csye6205 */
} /** end namespace neu */
} /** end namespace edu */

#endif /* INCLUDE_STORE_H_ */
