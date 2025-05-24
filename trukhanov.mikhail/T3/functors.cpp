#include "functors.hpp"
#include "polygon.hpp"

namespace trukhanov
{
  bool CompareByArea::operator()(const Polygon& lhs, const Polygon& rhs) const
  {
    double lhsArea = getArea(lhs);
    double rhsArea = getArea(rhs);
    return lhsArea < rhsArea;
  }

  bool LessArea::operator()(const Polygon& p) const
  {
    return getArea(p) < area;
  }

  double AccumulateAll::operator()(double sum, const Polygon& p) const
  {
    return sum + getArea(p);
  }

  bool CompareByVertexes::operator()(const Polygon& lhs, const Polygon& rhs) const
  {
    return compareByVertexes(lhs, rhs);
  }
}
