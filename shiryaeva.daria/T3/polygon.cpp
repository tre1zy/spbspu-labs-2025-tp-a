#include "polygon.hpp"
#include <algorithm>
#include <cmath>
#include <iterator>
#include <numeric>

namespace shiryaeva
{
  std::istream &operator>>(std::istream &in, Point &point)
  {
    std::istream::sentry s(in);
    if (!s) return in;

    Point tmp{};
    in >> DelimiterIO{'('} >> tmp.x >> DelimiterIO{';'} >> tmp.y >> DelimiterIO{')'};
    if (in)
    {
      point = tmp;
    }
    return in;
  }

  bool operator==(const Point &lhs, const Point &rhs)
  {
    return lhs.x == rhs.x && lhs.y == rhs.y;
  }

  bool operator<(const Point &lhs, const Point &rhs)
  {
    return (lhs.x < rhs.x) || (lhs.x == rhs.x && lhs.y < rhs.y);
  }

  std::istream &operator>>(std::istream &in, Polygon &polygon)
  {
    std::istream::sentry s(in);
    if (!s) return in;

    size_t count{};
    in >> count;
    if (!in || count < MIN_VERTEX_COUNT)
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    std::vector< Point > temp;
    using input_it_t = std::istream_iterator< Point >;
    std::copy_n(input_it_t{in}, count, std::back_inserter(temp));

    if (in)
    {
      polygon.points = temp;
    }

    return in;
  }

  bool isOdd(const Polygon &p)
  {
    return (p.points.size() % 2) == 1;
  }

  bool isEven(const Polygon &p)
  {
    return (p.points.size() % 2) == 0;
  }

  double getDistance(const Point &p1, const Point &p2)
  {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
  }

  Polygon buildTriangle(std::size_t i, const std::vector< Point > &pts)
  {
    return Polygon{ std::vector< Point >{ pts[0], pts[i + 1], pts[i + 2] } };
  }

  Polygon TriangleGenerator::operator()()
  {
    return buildTriangle(index++, points);
  }

  std::vector<Polygon> polygonToTriangles(const Polygon &p)
  {
    std::vector< Polygon > tris;
    tris.reserve(p.points.size() - 2);
    size_t index = 0;
    std::generate(tris.begin(), tris.end(), TriangleGenerator{index, p.points});
    return tris;
  }

  double getTriangleArea(const Polygon &triangle)
  {
    double a = getDistance(triangle.points[0], triangle.points[1]);
    double b = getDistance(triangle.points[1], triangle.points[2]);
    double c = getDistance(triangle.points[0], triangle.points[2]);
    double s = (a + b + c) / 2.0;
    return std::sqrt(s * (s - a) * (s - b) * (s - c));
  }

  double getPolygonArea(const Polygon &polygon)
  {
    std::vector< Polygon > triangles = polygonToTriangles(polygon);
    std::vector< double > areas;
    std::transform(triangles.begin(), triangles.end(), std::back_inserter(areas), getTriangleArea);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  double getTotalArea(const std::vector< Polygon > &polygons)
  {
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areas.begin(), getPolygonArea);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  bool compareByArea(const Polygon &a, const Polygon &b)
  {
    return getPolygonArea(a) < getPolygonArea(b);
  }

  bool compareByVertexes(const Polygon &a, const Polygon &b)
  {
    return a.points.size() < b.points.size();
  }

  bool comparePointByX(const Point &a, const Point &b)
  {
    return (a.x < b.x) || (a.x == b.x && a.y < b.y);
  }

  bool comparePointByY(const Point &a, const Point &b)
  {
    return (a.y < b.y) || (a.y == b.y && a.x < b.x);
  }
}
