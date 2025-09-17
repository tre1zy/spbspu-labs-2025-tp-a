#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <vector>

namespace cherkasov
{
  struct Point
  {
    int x;
    int y;

    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
  };

  struct Polygon
  {
    std::vector<Point> points;
    bool operator==(const Polygon& other) const;
  };

  double computeArea(const Polygon& poly);
  bool doPolygonsIntersect(const Polygon& a, const Polygon& b);
  bool hasRightAngle(const Polygon& poly);
}

#endif
