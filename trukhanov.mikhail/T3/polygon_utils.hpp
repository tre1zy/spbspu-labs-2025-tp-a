#ifndef POLYGON_UTILS_HPP
#define POLYGON_UTILS_HPP

#include <cstddef>
#include <algorithm>
#include "polygon.hpp"

namespace trukhanov
{
  struct isSize
  {
    size_t size;
    isSize(size_t s);
    bool operator()(const Polygon& p) const;
  };

  bool CompareByArea(const Polygon& lhs, const Polygon& rhs);

  struct LessArea
  {
    double area;
    bool operator()(const Polygon& p) const;
  };

  bool compareByVertexes(const Polygon& lhs, const Polygon& rhs);

  bool HasDuplicates(const Polygon& p);

  bool PolygonHasMinSize(const trukhanov::Polygon& p);

  double VectorProduct(const Point& a, const Point& b, const Point& c);

  struct MakeAngles
  {
    const std::vector< Point >& pts;
    Angle operator()(std::size_t i) const;
  };

  struct HasRightAngle
  {
    explicit HasRightAngle(const std::vector< Point >& pts) :
      pts_(pts)
    {}
    bool operator()(const Angle& ang) const;
    const std::vector< Point >& pts_;
  };
}

#endif
