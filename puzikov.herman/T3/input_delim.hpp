#ifndef INPUT_DELIMITER
#define INPUT_DELIMITER

#include <istream>

namespace puzikov
{
  struct DelimiterI
  {
    char exp;
  };
  std::istream &operator>>(std::istream &in, DelimiterI &&dest);
}
#endif
