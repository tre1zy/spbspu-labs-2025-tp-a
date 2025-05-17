#ifndef SHAPES_HPP
#define SHAPES_HPP
#include <vector>
#include <iostream>

namespace alymova
{
  struct Point
  {
    int x, y;
  };
  struct Polygon
  {
    std::vector< Point > points;
  };
  struct DelimiterIO
  {
    char exp;
  };

  std::istream& operator>>(std::istream& in, Point& point);
  std::istream& operator>>(std::istream& in, Polygon& polygon);
  std::istream& operator>>(std::istream& in, DelimiterIO&& object);
  std::ostream& operator<<(std::ostream& out, const Point& point);
  std::ostream& operator<<(std::ostream& out, const Polygon& polygon);

  double multPoints(const Point& point1, const Point& point2);
  bool isPolygonEven(const Polygon& polygon);
}
#endif
