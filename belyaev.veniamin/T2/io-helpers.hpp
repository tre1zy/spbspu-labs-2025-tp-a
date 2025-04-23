#ifndef IO_HELPERS_H
#define IO_HELPERS_H
#include <iostream>
#include <cstddef>
#include <string>

namespace belyaev
{
  struct DelimeterIO
  {
    char expected;
    bool caseSensitive;
    DelimeterIO(char expected_, bool caseSensitive_);
  };

  struct LabelIO
  {
    std::string expected;
  };

  struct DoubleEIO
  {
    double& value;
  };

  struct PairLLIO
  {
    std::pair< long long, unsigned long long >& value;
  };

  std::istream& operator>>(std::istream& in, const LabelIO&& dest);
  std::istream& operator>>(std::istream& in, const DelimeterIO&& dest);
  std::ostream& operator<<(std::ostream& out, const DoubleEIO& dest);
  std::ostream& operator<<(std::ostream& out, const PairLLIO& dest);
  std::istream& operator>>(std::istream& in, const DoubleEIO&& dest);
  std::istream& operator>>(std::istream& in, const PairLLIO&& dest);
}

#endif