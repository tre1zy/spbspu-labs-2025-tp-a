#include "ioGeometry.hpp"
#include <ioDelimiter.hpp>
#include <iterator>
#include <iostream>

std::istream & maslov::operator>>(std::istream & in, Point & dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterIO{'('} >> dest.x >> DelimiterIO{';'} >> dest.y >> DelimiterIO{')'};
  return in;
}

std::istream & maslov::operator>>(std::istream & in, Polygon & dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t num = 0;
  if (!(in >> num))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  for (size_t i = 0; i < num; ++i)
  {
    Point point;
    if (!(in >> point))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    dest.points.push_back(point);
  }
  return in;
}

std::ostream & maslov::operator<<(std::ostream & out, const Point & dest)
{
  return out << '(' << dest.x << ';' << dest.y << ')';
}

std::ostream & maslov::operator<<(std::ostream & out, const Polygon & dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  using oIterator = std::ostream_iterator< Point >;
  out << dest.points.size() << ' ';
  std::copy(dest.points.begin(), dest.points.end(), oIterator(out, " "));
  return out;
}
