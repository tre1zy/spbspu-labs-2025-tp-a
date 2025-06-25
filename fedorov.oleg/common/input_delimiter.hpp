#ifndef INPUT_DELIMITER
#define INPUT_DELIMITER

#include <iostream>

namespace fedorov
{
  struct DelimiterInput
  {
    char exp;
  };
  std::istream &operator>>(std::istream &in, DelimiterInput &&dest);

  struct DelimiterStr
  {
    const char *exp;
  };
  std::istream &operator>>(std::istream &in, DelimiterStr &&exp);

}
#endif
