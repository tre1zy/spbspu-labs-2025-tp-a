#include "polygon.hpp"
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <cmath>

namespace smirnov
{
  std::istream& operator>>(std::istream& in, Point& value)
  {
    std::istream::sentry guard(in);
    if (!guard)
    {
      return in;
    }

    char lparen = 0;
    char semicolon = 0;
    char rparen = 0;

    in >> lparen >> value.x >> semicolon >> value.y >> rparen;
    if (!in || lparen != '(' || semicolon != ';' || rparen != ')')
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, Polygon& value)
  {
    std::istream::sentry guard(in);
    if (!guard)
    {
      return in;
    }

    size_t n = 0;
    in >> n;
    if (n < 3)
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    using inputItT = std::istream_iterator< smirnov::Point >;
    std::vector< Point > vec;
    vec.reserve(n);
    std::copy_n(inputItT{ in }, n, std::back_inserter(vec));

    if (!in || vec.size() != n)
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    value = Polygon{ vec };
    return in;
  }

  bool operator==(const Point& a, const Point& b)
  {
    return a.x == b.x && a.y == b.y;
  }

  bool operator==(const Polygon& p1, const Polygon& p2)
  {
    return p1.points == p2.points;
  }

  double getArea(const Polygon& p)
  {
    double area = 0.0;
    for (size_t i = 0; i < p.points.size(); ++i)
    {
      const Point& a = p.points[i];
      const Point& b = p.points[(i + 1) % p.points.size()];
      area += (a.x * b.y - b.x * a.y);
    }
    return std::abs(area) / 2.0;
  }

  bool isPerpendicular(const Point& p1, const Point& p2, const Point& p3)
  {
    int dx1 = p2.x - p1.x;
    int dy1 = p2.y - p1.y;
    int dx2 = p3.x - p2.x;
    int dy2 = p3.y - p2.y;
    return dx1 * dx2 + dy1 * dy2 == 0;
  }

  bool isRect(const Polygon& p)
  {
    if (p.points.size() != 4)
    {
      return false;
    }
    return isPerpendicular(p.points[0], p.points[1], p.points[2]) &&
           isPerpendicular(p.points[1], p.points[2], p.points[3]) &&
           isPerpendicular(p.points[2], p.points[3], p.points[0]) &&
           isPerpendicular(p.points[3], p.points[0], p.points[1]);
  }

  bool isOddCountVertexes(const Polygon& p)
  {
    return (p.points.size() % 2) != 0;
  }

  bool isEvenCountVertexes(const Polygon& p)
  {
    return (p.points.size() % 2) == 0;
  }

  bool isNCountVertexes(const Polygon& p, size_t n)
  {
    return p.points.size() == n;
  }

  bool minArea(const Polygon& p1, const Polygon& p2)
  {
    return getArea(p1) < getArea(p2);
  }

  bool minVertexes(const Polygon& p1, const Polygon& p2)
  {
    return p1.points.size() < p2.points.size();
  }
}

