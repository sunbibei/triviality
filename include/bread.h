/*
 * bread.h
 *
 *  Created on: Mar 8, 2018
 *      Author: bibei
 */

#ifndef INCLUDE_BREAD_H_
#define INCLUDE_BREAD_H_

#include <item.h>

namespace edu {
namespace neu {
namespace csye6205 {

class Bread: public Item {
public:
  Bread(const std::string&, double, size_t);
  virtual ~Bread();

public:
  // friend std::ostream& operator<<(std::ostream&, const Bread&);
};

} /* namespace csye6205 */
} /** end namespace neu */
} /** end namespace edu */

#endif /* INCLUDE_BREAD_H_ */
