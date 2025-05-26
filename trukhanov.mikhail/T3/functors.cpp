#include "functors.hpp"
#include <algorithm>
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
  for (size_t i = 0; i < p.points.size(); ++i)
  {
    for (size_t j = i + 1; j < p.points.size(); ++j)
    {
      if (std::abs(p.points[i].x - p.points[j].x) &&
        std::abs(p.points[i].y - p.points[j].y))
      {
        return true;
      }
    }
  }
  return false;
}

bool trukhanov::PolygonHasMinSize::operator()(const trukhanov::Polygon& p) const
{
  return p.points.size() >= 3;
}
