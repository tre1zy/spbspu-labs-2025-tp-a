#ifndef DELIMETER_H
#define DELIMETER_H
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
