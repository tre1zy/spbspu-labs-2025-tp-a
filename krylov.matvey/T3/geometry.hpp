#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP
#include <vector>
#include <iostream>

namespace krylov
{
  struct Point
  {
    int x;
    int y;
  };
  std::istream& operator>>(std::istream& in, Point& p);

  struct Polygon
  {
    std::vector< Point > points;
  };
  std::istream& operator>>(std::istream& in, Polygon& polygon);
}

#endif
