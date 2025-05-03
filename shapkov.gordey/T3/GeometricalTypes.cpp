#include "GeometricalTypes.hpp"
#include <delimiter.hpp>

using check = shapkov::DelimiterIO;

std::istream& shapkov::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  int x = 0, y = 0;
  in >> check{ '(' };
  in >> x;
  in >> check{ ',' };
  in >> y;
  in >> check{ ')' };
  if (in)
  {
    point.x = x;
    point.y = y;
  }
  return in;
}

std::ostream& shapkov::operator<<(std::ostream& out, const Point& point)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  return (out << '(' << point.x << ',' << point.y << ')');
}
