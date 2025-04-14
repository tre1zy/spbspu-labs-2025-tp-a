#ifndef IO_HELPERS_H
#define IO_HELPERS_H
#include <iostream>
#include <cstddef>
#include <string>

namespace belyaev {
  struct DoubleIO {
    double& num_;
  };

  std::ostream& operator<<(std::ostream& out, const DoubleIO& dbl);
}

#endif