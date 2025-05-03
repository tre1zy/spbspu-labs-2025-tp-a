#include "GeometricalTypes.hpp"
#include <algorithm>
#include <functional>
#include <iterator>
#include <limits>
#include <numeric>
#include "math.h"
#include <delimiter.hpp>

using check = shapkov::DelimiterIO;

std::istream& shapkov::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  int x = 0, y = 0;
  in >> check{ '(' };
  in >> x;
  in >> check{ ';' };
  in >> y;
  in >> check{ ')' };
  if (in)
  {
    point.x = x;
    point.y = y;
  }
  return in;
}

std::ostream& shapkov::operator<<(std::ostream& out, const Point& point)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  return (out << " (" << point.x << ';' << point.y << ')');
}

std::istream& shapkov::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  };
  int vertexes = 0;
  in >> vertexes;
  std::vector< Point > temp;
  std::copy_n(std::istream_iterator< Point >(in), vertexes, std::back_inserter(temp));
  if (in)
  {
    polygon.points = temp;
  }
  return in;
}

std::ostream& shapkov::operator<<(std::ostream& out, const Polygon& polygon)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  out << polygon.points.size();
  std::copy(std::begin(polygon.points), std::end(polygon.points), std::ostream_iterator< Point >(out, ""));
  return out;
}

double shapkov::getDistance(const Point& p1, const Point& p2)
{
  return sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
}

double shapkov::getAreaOfTriangle(const Polygon& p)
{
  double sideA = getDistance(p.points[0], p.points[1]);
  double sideB = getDistance(p.points[2], p.points[1]);
  double sideC = getDistance(p.points[0], p.points[2]);
  double halfPerimeter = (sideA + sideB + sideC) / 2;
  return sqrt(halfPerimeter * (halfPerimeter - sideA) * (halfPerimeter - sideB) * (halfPerimeter - sideC));
}

shapkov::Polygon shapkov::makeTriangle(size_t i, const std::vector< Point >& pts)
{
  return Polygon{ std::vector<Point>{ pts[0], pts[i + 1], pts[i + 2] } };
}

void shapkov::polygonToTriangles(const Polygon& p, std::vector< Polygon >& triangles)
{
  size_t size = p.points.size() - 2;
  std::vector< size_t > indices(size);
  std::iota(indices.begin(), indices.end(), 0);
  using namespace std::placeholders;
  std::transform(
    indices.begin(), indices.end(),
    std::back_inserter(triangles),
    std::bind(makeTriangle, _1, std::cref(p.points))
  );
}

double shapkov::getArea(const Polygon& p)
{
  double area = 0;
  std::vector< Polygon > triangles;
  polygonToTriangles(p, triangles);
  std::vector< double > areas;
  std::transform(triangles.begin(), triangles.end(), std::back_inserter(areas), getAreaOfTriangle);
  area = std::accumulate(areas.begin(), areas.end(), 0.0);
  return area;
}

bool shapkov::isEven(const Polygon& p)
{
  return p.points.size() % 2 == 0;
}

bool shapkov::isOdd(const Polygon& p)
{
  return !isEven(p);
}
