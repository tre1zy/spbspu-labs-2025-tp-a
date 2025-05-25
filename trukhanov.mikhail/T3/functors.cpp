#include "functors.hpp"
#include <algorithm>
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

  bool HasDuplicates::operator()(const Polygon& p) const
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

    bool PolygonEqual::operator()(const Polygon& a, const Polygon& b) const
    {
      if (a.points.size() != b.points.size())
      {
        return false;
      }
      return std::equal(a.points.begin(), a.points.end(), b.points.begin());
    }

      bool PolygonHasMinSize::operator()(const trukhanov::Polygon& p) const
      {
        return p.points.size() >= 3;
      }
}
