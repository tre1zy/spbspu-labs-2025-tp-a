#include "delimiter.hpp"
#include <istream>

std::istream kushekbaev::operator>>(std::istream& in, DelimiterIO&& obj)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return in;
  }
  char c = 0;
  in >> c;
  if (in && std::tolower(c) != obj.exp)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
