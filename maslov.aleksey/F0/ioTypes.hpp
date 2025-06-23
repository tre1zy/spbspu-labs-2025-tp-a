#ifndef IO_TYPES_HPP
#define IO_TYPES_HPP

#include <iostream>

namespace maslov
{
  using Words = std::pair< std::string, int >;
  std::ostream & operator<<(std::ostream & out, const Words & wordPair);
}

#endif
