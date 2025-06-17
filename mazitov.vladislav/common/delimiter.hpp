#ifndef DELIMITER_HPP
#define DELIMITER_HPP

#include <iostream>

namespace mazitov
{
  struct Delimiter
  {
    char exp;
  };
  std::istream &operator>>(std::istream &in, Delimiter &&dest);
}

#endif
