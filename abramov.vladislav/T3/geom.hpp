#ifndef GEOM_HPP
#define GEOM_HPP
#include <vector>
#include <iostream>

namespace abramov
{
  struct Point
  {
    int x;
    int y;
  };
  std::istream &operator>>(std::istream &in, Point &p);

  struct Polygon
  {
    std::vector< Point > points;
  };
  std::istream &operator>>(std::istream &in, Polygon &polygon);

  double getArea(const Polygon &polygon);
  double areaEven(double s, const Polygon &polygon);
  double areaOdd(double s, const Polygon &polygon);
  double areaMean(const std::vector< Polygon > &polygons);
  double areaVertexes(double s, const Polygon &polygon, size_t vert);
  bool maxArea(const Polygon &p1, const Polygon& p2);
  bool maxVertexes(const Polygon &p1, const Polygon &p2);
};


#endif
