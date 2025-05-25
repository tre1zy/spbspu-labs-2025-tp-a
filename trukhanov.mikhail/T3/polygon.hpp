#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>

namespace trukhanov
{
  struct Point
  {
    int x, y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };

  inline bool operator==(const Point& lhs, const Point& rhs)
  {
    return lhs.x == rhs.x && lhs.y == rhs.y;
  }

  double getArea(const Polygon& p);
  bool isEven(const Polygon& p);
  bool isOdd(const Polygon& p);
  bool compareByVertexes(const Polygon& p1, const Polygon& p2);
  bool isRight(const Polygon& p);
}

#endif
