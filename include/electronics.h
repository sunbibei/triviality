/*
 * electronics.h
 *
 *  Created on: Mar 8, 2018
 *      Author: bibei
 */

#ifndef ELECTRONICS_H_
#define ELECTRONICS_H_

#include <item.h>

namespace edu {
namespace neu {
namespace csye6205 {

class Electronics: public Item {
public:
  Electronics(const std::string&, double, size_t);
  virtual ~Electronics();

public:
  // friend std::ostream& operator<<(std::ostream&, const Electronics&);
};

} /* namespace csye6205 */
} /** end namespace neu */
} /** end namespace edu */

#endif /* ELECTRONICS_H_ */
