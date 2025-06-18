#include "intersections_command.hpp"
#include <algorithm>

namespace
{
  using mazitov::Point;
  using mazitov::Polygon;

  bool onSegment(Point a, Point b, Point p)
  {
    bool cond1 = std::min(a.x, b.x) <= p.x;
    bool cond2 = p.x <= std::max(a.x, b.x);
    bool cond3 = std::min(a.y, b.y) <= p.y;
    bool cond4 = p.y <= std::max(a.y, b.y);

    return cond1 && cond2 && cond3 && cond4;
  }

  int ccw(Point a, Point b, Point c)
  {
    int area = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    if (area > 0)
    {
      return 1;
    }
    if (area < 0)
    {
      return -1;
    }
    return 0;
  }

  bool segmentsIntersect(Point p1, Point p2, Point q1, Point q2)
  {
    int d1 = ccw(p1, p2, q1);
    int d2 = ccw(p1, p2, q2);
    int d3 = ccw(q1, q2, p1);
    int d4 = ccw(q1, q2, p2);

    if (d1 * d2 < 0 && d3 * d4 < 0)
    {
      return true;
    }

    bool cond1 = (d1 == 0 && onSegment(p1, p2, q1));
    bool cond2 = (d2 == 0 && onSegment(p1, p2, q2));
    bool cond3 = (d3 == 0 && onSegment(q1, q2, p1));
    bool cond4 = (d4 == 0 && onSegment(q1, q2, p2));

    return (cond1 || cond2 || cond3 || cond4);
  }

  struct RayCastCheck
  {
    RayCastCheck(const std::vector< Point > &poly, const Point &pt):
      polygon(poly),
      point(pt),
      i(0),
      j(poly.size() - 1)
    {}

    bool operator()(const Point &a)
    {
      const Point &b = polygon[j];

      bool cond1 = (a.y > point.y) != (b.y > point.y);
      double intersectX = (b.x - a.x) * (point.y - a.y) / (b.y - a.y + 1e-10) + a.x;

      j = i++;
      return cond1 && point.x < intersectX;
    }

    const std::vector< Point > &polygon;
    const Point &point;
    std::size_t i;
    std::size_t j;
  };

  bool pointInPolygon(const std::vector< Point > &polygon, Point p)
  {
    RayCastCheck checker(polygon, p);
    std::size_t crossings = std::count_if(polygon.begin(), polygon.end(), checker);
    return crossings % 2 == 1;
  }

  struct PolygonIntersects
  {
    explicit PolygonIntersects(const Polygon &target):
      polygon(target)
    {}

    bool operator()(const Polygon &other) const
    {
      if (polygon.points == other.points)
      {
        return false;
      }

      if (hasEdgeIntersection(polygon, other))
      {
        return true;
      }

      if (!polygon.points.empty() && pointInPolygon(other.points, polygon.points.front()))
      {
        return true;
      }

      if (!other.points.empty() && pointInPolygon(polygon.points, other.points.front()))
      {
        return true;
      }

      return false;
    }

    static bool hasEdgeIntersection(const Polygon &a, const Polygon &b)
    {
      const std::vector< Point > &ptsA = a.points;
      const std::vector< Point > &ptsB = b.points;

      return std::any_of(ptsA.begin(), ptsA.end(), EdgeChecker(ptsA, ptsB));
    }

    struct EdgeChecker
    {
      EdgeChecker(const std::vector< Point > &a, const std::vector< Point > &b):
        polyA(a),
        polyB(b)
      {}

      bool operator()(const Point &pa_i) const
      {
        std::size_t i = &pa_i - &polyA[0];
        const Point &pa_j = polyA[(i + 1) % polyA.size()];

        return std::any_of(polyB.begin(), polyB.end(), SegmentPairTester(pa_i, pa_j, polyB));
      }

      const std::vector< Point > &polyA;
      const std::vector< Point > &polyB;

      struct SegmentPairTester
      {
        SegmentPairTester(const Point &a1, const Point &a2, const std::vector< Point > &b):
          a1(a1),
          a2(a2),
          polyB(b)
        {}

        bool operator()(const Point &pb_i) const
        {
          std::size_t i = &pb_i - &polyB[0];
          const Point &pb_j = polyB[(i + 1) % polyB.size()];
          return segmentsIntersect(a1, a2, pb_i, pb_j);
        }

        const Point &a1;
        const Point &a2;
        const std::vector< Point > &polyB;
      };
    };

    const Polygon &polygon;
  };
}

std::size_t mazitov::getIntersections(const std::vector< Polygon > &polygons, const Polygon &polygon)
{
  return std::count_if(polygons.begin(), polygons.end(), PolygonIntersects(polygon));
}
