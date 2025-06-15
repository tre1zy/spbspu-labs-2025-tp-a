#include "Point.h"
#include <iomanip>
#include <iostream>
#include <Delimiter.h>

bool horoshilov::Point::operator==(const Point& other) const
{
  return x == other.x && y == other.y;
}
bool horoshilov::Point::operator<(const Point& other) const
{
  return x < other.x || (x == other.x && y < other.y);
}

std::istream& kostyukov::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  Point temp;
  {
    using del = DelimiterIO;
    in >> del{ '(' } >> temp.x >> del{ ';' } >> temp.y >> del{ ')' };
    if (in)
    {
      point = temp;
    }
  }
  return in;
}
