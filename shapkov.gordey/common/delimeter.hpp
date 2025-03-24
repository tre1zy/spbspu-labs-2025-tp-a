#ifndef DELIMETER_HPP
#define DELIMETER_HPP
#include <iostream>

namespace shapkov
{
  struct delimiterIO
  {
    char exp;
  };
  std::istream& operator>>(std::istream& in, delimiterIO&& dest);
}
#endif
