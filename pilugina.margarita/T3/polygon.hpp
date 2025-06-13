#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <istream>
#include <vector>
#include <delimiterIO.hpp>

namespace pilugina
{
  struct Point
  {
    int x, y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };

  std::istream &operator>>(std::istream &in, Point &dest);
  std::istream &operator>>(std::istream &in, Polygon &dest);

  bool operator==(const Point &lhs, const Point &rhs);
}

#endif
