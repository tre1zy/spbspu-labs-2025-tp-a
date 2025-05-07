#include "geom.hpp"
#include <iterator>
#include <algorithm>

namespace
{
  struct DelimiterIO
  {
    char delim;
  };
  std::istream &operator>>(std::istream &in, DelimiterIO &&dest)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    char c = '0';
    in >> c;
    if (in && (c != dest.delim))
    {
      throw std::logic_error("wrong symbol\n");
    }
    return in;
  }
}

std::istream &abramov::operator>>(std::istream &in, Point &p)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  Point tmp{ 0, 0 };
  in >> DelimiterIO{ '(' };
  in >> tmp.x;
  in >> DelimiterIO{ ';' };
  in >> tmp.y;
  in >> DelimiterIO{ ')' };
  if (in)
  {
    p = tmp;
  }
  return in;
}

std::istream &abramov::operator>>(std::istream &in, Polygon &polygon)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  size_t k = 0;
  in >> k;
  if (k < 3)
  {
    throw std::logic_error("wrong size\n");
  }
  std::copy_n(std::istream_iterator< Point >(in), k, std::back_inserter(polygon.points));
  return in;
}
