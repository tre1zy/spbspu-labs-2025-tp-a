#include "point.h"
#include <iomanip>
#include <iostream>

namespace kharlamov
{
  bool Point::operator==(const Point& other) const
  {
    return x == other.x && y == other.y;
  }
  std::istream& operator>>(std::istream& in, Point& point)
  {
    char sep;
    if (!(in >> sep >> point.x >> sep >> point.y >> sep) || sep != ')')
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }
}
