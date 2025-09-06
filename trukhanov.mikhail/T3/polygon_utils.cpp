#include "polygon_utils.hpp"
#include <algorithm>
#include <cmath>
#include "polygon.hpp"

trukhanov::isSize::isSize(size_t s) : size(s) {}

bool trukhanov::isSize::operator()(const trukhanov::Polygon& p) const
{
  return p.points.size() == size;
}

bool trukhanov::CompareByArea(const Polygon& lhs, const Polygon& rhs)
{
  double lhsArea = getArea(lhs);
  double rhsArea = getArea(rhs);
  return lhsArea < rhsArea;
}

bool trukhanov::LessArea::operator()(const Polygon& p) const
{
  return getArea(p) < area;
}

bool trukhanov::compareByVertexes(const Polygon& lhs, const Polygon& rhs)
{
  return lhs.points.size() < rhs.points.size();
}

bool trukhanov::HasDuplicates(const Polygon& p)
{
  if (p.points.size() < 2)
  {
    return false;
  }
  std::vector< Point > sortedPoints = p.points;
  std::sort(sortedPoints.begin(), sortedPoints.end());
  return std::adjacent_find(sortedPoints.begin(), sortedPoints.end()) != sortedPoints.end();
}

bool trukhanov::PolygonHasMinSize(const trukhanov::Polygon& p)
{
  return p.points.size() >= 3;
}

double trukhanov::VectorProduct(const Point& a, const Point& b, const Point& c)
{
  return 0.5 * std::abs(static_cast<double>((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y)));
}

trukhanov::Angle trukhanov::MakeAngles::operator()(std::size_t i) const
{
  std::size_t n = pts.size();
  return { pts[i], pts[(i + 1) % n], pts[(i + 2) % n] };
}

bool trukhanov::HasRightAngle::operator()(const Angle& ang) const
{
  int x1 = ang.a.x - ang.b.x;
  int y1 = ang.a.y - ang.b.y;
  int x2 = ang.c.x - ang.b.x;
  int y2 = ang.c.y - ang.b.y;
  return x1 * x2 + y1 * y2 == 0;
}
