#include "polygon_utils.hpp"
#include <algorithm>
#include <cmath>
#include "polygon.hpp"

bool trukhanov::isSize::operator()(const trukhanov::Polygon& p) const
{
  return p.points.size() == size;
}

bool trukhanov::CompareByArea::operator()(const Polygon& lhs, const Polygon& rhs) const
{
  double lhsArea = getArea(lhs);
  double rhsArea = getArea(rhs);
  return lhsArea < rhsArea;
}

bool trukhanov::LessArea::operator()(const Polygon& p) const
{
  return getArea(p) < area;
}

bool trukhanov::CompareByVertexes::operator()(const Polygon& lhs, const Polygon& rhs) const
{
  return compareByVertexes(lhs, rhs);
}

bool trukhanov::HasDuplicates::operator()(const Polygon& p) const
{
  if (p.points.size() < 2)
  {
    return false;
  }
  std::vector< Point > sortedPoints = p.points;
  std::sort(sortedPoints.begin(), sortedPoints.end());
  return std::adjacent_find(sortedPoints.begin(), sortedPoints.end()) != sortedPoints.end();
}

bool trukhanov::PolygonHasMinSize::operator()(const trukhanov::Polygon& p) const
{
  return p.points.size() >= 3;
}

double trukhanov::VectorProduct::operator()(const Point& a, const Point& b, const Point& c) const
{
  return 0.5 * std::abs(static_cast< double >((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y)));
}

bool trukhanov::HasRightAngle::operator()(const Angle& ang) const
{
  int x1 = ang.a.x - ang.b.x;
  int y1 = ang.a.y - ang.b.y;
  int x2 = ang.c.x - ang.b.x;
  int y2 = ang.c.y - ang.b.y;
  return x1 * x2 + y1 * y2 == 0;
}
