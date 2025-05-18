#ifndef POLYGON_OPERATIONS_HPP
#define POLYGON_OPERATIONS_HPP

#include <iostream>
#include <algorithm>
#include <numeric>
#include "ioGeometry.hpp"

namespace maslov
{
  struct IsEven
  {
    bool operator()(const Polygon & polygon) const
    {
      return polygon.points.size() % 2 == 0;
    }
  };

  struct IsOdd
  {
    bool operator()(const Polygon & polygon) const
    {
      return polygon.points.size() % 2 != 0;
    }
  };

  struct VectorProduct
  {
    Point p0;
    double operator()(const Point & p1, const Point & p2) const
    {
      double d1 = (p1.x - p0.x) * (p2.y - p0.y);
      double d2 = (p2.x - p0.x) * (p1.y - p0.y);
      return 0.5 * std::abs(d1 - d2);
    }
  };

  struct AreaCalculator
  {
    double operator()(const Polygon & polygon) const
    {
    if (polygon.points.size() < 3)
    {
      return 0.0;
    }
    VectorProduct calc{polygon.points[0]};
    std::vector< double > areas;
    const auto & begin = polygon.points.begin();
    const auto & end = polygon.points.end();
    std::transform(begin + 1, end - 1, begin + 2, std::back_inserter(areas), calc);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
    }
  };

  struct SameVertexes
  {
    size_t num;
    bool operator()(const Polygon & polygon) const
    {
      return polygon.points.size() == num;
    }
  };

  struct VertexesComparator
  {
    bool operator()(const Polygon & poly1, const Polygon & poly2) const
    {
      return poly1.points.size() < poly2.points.size();
    }
  };

  int scalarProduct(const Point & a, const Point & b, const Point & c)
  {
    int vectorX1 = b.x - a.x;
    int vectorY1 = b.y - a.y;
    int vectorX2 = c.x - b.x;
    int vectorY2 = c.y - b.y;
    return vectorX1 * vectorX2 + vectorY1 * vectorY2;
  }

  struct IsRectangle
  {
    bool operator()(const Polygon & polygon) const
    {
      if (polygon.points.size() != 4)
      {
        return false;
      }
      const auto & p = polygon.points;
      bool check = (scalarProduct(p[0], p[1], p[2]) == 0) && (scalarProduct(p[1], p[2], p[3]) == 0);
      return check && (scalarProduct(p[2], p[3], p[0]) == 0);
    }
  };

  struct SamePolygon
  {
    const Polygon inPolygon;
    bool operator()(const Polygon & polygon) const
    {
      return polygon == inPolygon;
    }
  };

  struct EchoTransformer
  {
    const Polygon inPolygon;
    std::vector< Polygon > operator()(const Polygon & polygon) const
    {
      if (polygon == inPolygon)
      {
        return {polygon, polygon};
      }
      return {polygon};
    }
  };

  void dublicatePolygon(const std::vector< Polygon > & polygons, const Polygon & inPolygon,
      std::vector< Polygon > & result)
  {
    for (const auto & poly : polygons)
    {
      result.push_back(poly);
      if (poly == inPolygon)
      {
        result.push_back(poly);
      }
    }
  }
}

#endif
