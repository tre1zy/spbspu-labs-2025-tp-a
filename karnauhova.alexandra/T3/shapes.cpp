#include "shapes.hpp"
#include <algorithm>
#include <iterator>

std::istream& karnauhova::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (std::tolower(c) != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& karnauhova::operator>>(std::istream& in, Point&& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  using del =  DelimiterIO;
  in >> del{ '(' } >> point.x >> del{ ';' } >> point.y >> del{ ')' };
  return in;
}

std::istream& karnauhova::operator>>(std::istream& in, Polygon& pol)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  using point = Point;
  size_t count = 0;
  in >> count;
  if (!in || count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > temp(count);
  std::copy_n(std::istream_iterator< Point >(in), count, temp.begin());
  if (in)
  {
    pol.points = temp;
  }
  return in;
}
