#include "shapes.hpp"
#include <iterator>
#include <algorithm>

std::istream& finaev::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> delimiterIO{'('} >> point.x >> delimiterIO{';'} >> point.y >> delimiterIO{')'};
  return in;
}

std::ostream& finaev::operator<<(std::ostream& out, const Point& point)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << "(" << point.x << ";" << point.y << ")";
  return out;
}


std::istream& finaev::operator>>(std::istream& in, Polygon& poly)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t count = 0;
  if (!(in >> count) || count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  poly.points.clear();
  poly.points.reserve(count);
  for (size_t i = 0; i < count; ++i)
  {
    Point p;
    if (!(in >> p))
    {
      poly.points.clear();
      in.setstate(std::ios::failbit);
      return in;
    }
    if (in.fail())
    {
      poly.points.clear();
      in.setstate(std::ios::failbit);
      return in;
    }
    poly.points.push_back(p);
  }
  return in;
}

std::ostream& finaev::operator<<(std::ostream& out, const Polygon& poly)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  std::copy(poly.points.begin(), poly.points.end(), std::ostream_iterator< Point >{ out, " " });
  return out;
}
