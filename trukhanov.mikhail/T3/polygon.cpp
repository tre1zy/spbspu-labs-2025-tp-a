#include "polygon.hpp"
#include <cmath>
#include <numeric>
#include <vector>
#include "functors.hpp"

namespace trukhanov
{
  double getArea(const Polygon& polygon)
  {
    std::size_t n = polygon.points.size();
    if (n < 3)
      return 0.0;

    std::vector<std::size_t> indices(n - 2);
    std::iota(indices.begin(), indices.end(), 1);

    PolygonAreaSum summer(polygon);

    double area = std::accumulate(indices.begin(), indices.end(), 0.0, summer);
    return area;
  }

  bool isEven(const Polygon& polygon)
  {
    return polygon.points.size() != 0 && polygon.points.size() % 2 == 0;
  }

  bool isOdd(const Polygon& polygon)
  {
    return polygon.points.size() != 0 && polygon.points.size() % 2 != 0;
  }

  bool compareByVertexes(const Polygon& lhs, const Polygon& rhs)
  {
    return lhs.points.size() < rhs.points.size();
  }

  static int dot(const Point& a, const Point& b)
  {
    return a.x * b.x + a.y * b.y;
  }

  static Point sub(const Point& a, const Point& b)
  {
    return { a.x - b.x, a.y - b.y };
  }

  static bool isRightAngle(const Point& a, const Point& b, const Point& c)
  {
    Point ab = sub(b, a);
    Point bc = sub(c, b);
    return dot(ab, bc) == 0;
  }

  bool isRight(const Polygon& polygon)
  {
    size_t n = polygon.points.size();
    if (n < 3)
    {
      return false;
    }

    for (size_t i = 0; i < n; ++i)
    {
      const Point& a = polygon.points[i];
      const Point& b = polygon.points[(i + 1) % n];
      const Point& c = polygon.points[(i + 2) % n];
      if (isRightAngle(a, b, c))
      {
        return true;
      }
    }

    return false;
  }

  bool operator<(const Point& lhs, const Point& rhs)
  {
    return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
  }

  bool operator==(const Point& lhs, const Point& rhs)
  {
    return lhs.x == rhs.x && lhs.y == rhs.y;
  }
}
