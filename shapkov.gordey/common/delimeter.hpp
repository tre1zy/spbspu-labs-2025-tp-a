#ifndef DELIMETER_HPP
#define DELIMETER_HPP
#include <iostream>

namespace shapkov
{
  struct DelimiterIO
  {
    char exp;
  };
  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
}
#endif
