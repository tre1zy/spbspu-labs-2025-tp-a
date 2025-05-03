#include "polygon.hpp"
#include <ios>
#include <istream>
#include <iterator>
#include "scopeGuard.hpp"
#include "delimeters.hpp"

std::istream& kiselev::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  detail::ScopeGuard scope(in);
  Point temp;
  in >> DelimeterIO{ '(' } >> temp.x >> DelimeterIO{ ';' } >> temp.y >> DelimeterIO{ ')' };
  if (in)
  {
    point = temp;
  }
  return in;
}

std::istream& kiselev::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  detail::ScopeGuard scope(in);
  size_t count = 0;
  in >> count;
  if (count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  Polygon temp;
  temp.points.reserve(count);
  std::copy(std::istream_iterator< Point >(in), std::istream_iterator< Point >(), std::back_inserter(temp.points));
  if (in)
  {
    polygon = temp;
  }
  return in;
}
