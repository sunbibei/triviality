/*
 * healthy_bread.h
 *
 *  Created on: Mar 8, 2018
 *      Author: bibei
 */

#ifndef INCLUDE_HEALTHY_BREAD_H_
#define INCLUDE_HEALTHY_BREAD_H_

#include <item.h>

namespace edu {
namespace neu {
namespace csye6205 {

class HealthyBread: public Item {
public:
  HealthyBread(const std::string& _n, double _p, size_t _num, double _c);
  virtual ~HealthyBread();

public:
  friend std::ostream& operator<<(std::ostream&, const HealthyBread&);

protected:
  double calories_;
};

} /* namespace csye6205 */
} /** end namespace neu */
} /** end namespace edu */

#endif /* INCLUDE_HEALTHY_BREAD_H_ */
