#include "point.h"
#include <iomanip>
#include <iostream>
#include "delimetr.h"

namespace kharlamov
{
  bool Point::operator==(const Point& other) const
  {
    return x == other.x && y == other.y;
  }
  std::istream& kharlamov::operator>>(std::istream& in, Point& point)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    in >> DelimeterIO{ '(' } >> point.x >> DelimeterIO{ ';' } >> point.y >> DelimeterIO{ ')' };
    return in;
  }
}
