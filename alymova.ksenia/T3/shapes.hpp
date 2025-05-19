#ifndef SHAPES_HPP
#define SHAPES_HPP
#include <vector>
#include <iostream>

namespace alymova
{
  struct DelimiterIO
  {
    char exp;
  };
  struct Point
  {
    int x, y;
  };
  struct Polygon
  {
    std::vector< Point > points;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& object);
  std::istream& operator>>(std::istream& in, Point& point);
  std::istream& operator>>(std::istream& in, Polygon& polygon);
  std::ostream& operator<<(std::ostream& out, const Point& point);
  std::ostream& operator<<(std::ostream& out, const Polygon& polygon);

  bool insertIfNotNextEnter(std::istream& in, const Point& point, std::vector< Point >& dop);
}
#endif
