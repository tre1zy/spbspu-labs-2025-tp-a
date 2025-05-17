#include "shapes.hpp"
#include <iterator>
#include <algorithm>

std::istream& alymova::operator>>(std::istream& in, DelimiterIO&& object)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  char delim;
  if (in >> delim && delim != object.exp)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
