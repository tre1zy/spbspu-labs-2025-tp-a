#ifndef DICT_UTILS_HPP
#define DICT_UTILS_HPP
#include <iostream>
#include <list>

namespace alymova
{
  std::istream& operator>>(std::istream& in, std::list< std::string >& list);
  std::ostream& operator<<(std::ostream& out, const std::list< std::string >& list);
}
#endif
