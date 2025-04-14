#ifndef IO_HELPERS_H
#define IO_HELPERS_H
#include <iostream>
#include <cstddef>
#include <string>

namespace belyaev {
  struct DoubleIO {
    double& value;
  };

  struct PairLLIO {
    std::pair< long long, unsigned long long >& value;
  };

  std::ostream& operator<<(std::ostream&, const DoubleIO&);
  std::ostream& operator<<(std::ostream&, const PairLLIO&);
}

#endif