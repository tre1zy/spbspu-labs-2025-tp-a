#ifndef DELIMITER_HPP
#define DELIMITER_HPP

namespace kushekbaev
{
  struct DelimiterIO
  {
    char exp;
  };
  std::istream& operator>>(std::istream& in, DelimiterIO&& obj);
}

#endif
