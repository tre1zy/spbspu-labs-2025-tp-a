#include "polygon_utils.hpp"
#include <algorithm>
#include <cmath>
#include "polygon.hpp"

bool trukhanov::isSize::operator()(const Polygon& p) const
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

double trukhanov::AccumulateAll::operator()(double sum, const Polygon& p) const
{
  return sum + getArea(p);
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

double trukhanov::TriangleArea::operator()(const Point& a, const Point& b, const Point& c) const
{
  return std::abs((a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / 2.0);
}

double trukhanov::VectorProduct::operator()(const Point& a, const Point& b, const Point& c) const
{
  return 0.5 * std::abs(static_cast< double >((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y)));
}

bool trukhanov::HasRightAngle::operator()(size_t i) const
{
  size_t n = pts_.size();
  const Point& a = pts_[(i + n - 1) % n];
  const Point& b = pts_[i];
  const Point& c = pts_[(i + 1) % n];

  int abx = b.x - a.x;
  int aby = b.y - a.y;
  int bcx = c.x - b.x;
  int bcy = c.y - b.y;

  return (abx * bcx + aby * bcy) == 0;
}
