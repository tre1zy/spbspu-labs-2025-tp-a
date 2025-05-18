#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include <iosfwd>

namespace rychkov
{
  struct Point
  {
    double x, y;
  };
  struct Polygon
  {
    std::vector< Point > vertexes;
  };
  std::istream& operator>>(std::istream& in, Point& point);
  std::istream& operator>>(std::istream& in, Polygon& polygon);
}

#endif
