/*
 * item.h
 *
 *  Created on: Mar 8, 2018
 *      Author: bibei
 */

#ifndef INCLUDE_ITEM_H_
#define INCLUDE_ITEM_H_

#include <string>
#include <iostream>

namespace edu {
namespace neu {
namespace csye6205 {

class Item {
public:
  /*!
   * @brief The constructor, (name, price, number)
   */
  Item(const std::string&, double, size_t);
  virtual ~Item();

public:
  friend std::ostream& operator<<(std::ostream&, const Item&);

protected:
  std::string name_;
  double      price_;
  size_t      number_;
};

} /** namespace csye6205 */
} /** end namespace neu */
} /** end namespace edu */

#endif /* INCLUDE_ITEM_H_ */
