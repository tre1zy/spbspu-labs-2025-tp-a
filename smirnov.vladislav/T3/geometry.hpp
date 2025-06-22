#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP
#include <vector>
#include <iosfwd>

namespace geom
{
  struct Point
  {
    int x;
    int y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };

  bool operator==(const Point& p1, const Point& p2);
  bool operator>(const Point& p, const Point& other);
  bool operator<(const Point& p, const Point& other);
  std::istream& operator>>(std::istream& in, Point& p);
  std::istream& operator>>(std::istream& in, Polygon& poly);
  double getPolygonArea(const Polygon& poly);
}

#endif
