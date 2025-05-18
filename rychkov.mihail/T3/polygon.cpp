#include "polygon.hpp"

#include <iostream>
#include <iterator>
#include <algorithm>
#include <skip_any_of.hpp>

std::istream& rychkov::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return in >> iofmt::anyof("(") >> point.x >> iofmt::anyof(";") >> point.y >> iofmt::anyof(")");
}
std::istream& rychkov::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t count = 0;
  if (!(in >> count) && (count < 3))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  polygon.vertexes.clear();
  std::copy_n(std::istream_iterator< Point >{in}, count, std::back_inserter(polygon.vertexes));
  return in;
}
