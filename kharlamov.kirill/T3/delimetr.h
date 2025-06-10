#ifndef DELIMETERS_HPP
#define DELIMETERS_HPP
#include <string>

namespace kiselev
{
  struct DelimeterIO
  {
    char exp;
  };

std::istream& operator>>(std::istream& input, DelimeterIO&& dest);
}
#endif
