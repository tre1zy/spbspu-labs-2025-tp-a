#ifndef IO_TYPES_HPP
#define IO_TYPES_HPP

#include <iosfwd>
#include "dataStruct.hpp"

namespace maslov
{
  std::ostream & maslov::operator<<(std::ostream & out, const DataStruct & data);
}

#endif
