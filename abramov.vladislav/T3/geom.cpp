#include "geom.hpp"
#include <iterator>
#include <algorithm>
#include <numeric>
namespace
{
  struct DelimiterIO
  {
    char delim;
  };

  std::istream &operator>>(std::istream &in, DelimiterIO &&dest)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    char c = '0';
    in >> c;
    if (in && (c != dest.delim))
    {
      throw std::logic_error("wrong symbol\n");
    }
    return in;
  }

  int pointsSum(const abramov::Point &p1, const abramov::Point &p2)
  {
    return p1.x * p2.y - p2.x * p1.y;
  }

  bool isEven(const abramov::Polygon &polygon)
  {
    return polygon.points.size() % 2 == 0;
  }
}

std::istream &abramov::operator>>(std::istream &in, Point &p)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  Point tmp{ 0, 0 };
  in >> DelimiterIO{ '(' };
  in >> tmp.x;
  in >> DelimiterIO{ ';' };
  in >> tmp.y;
  in >> DelimiterIO{ ')' };
  if (in)
  {
    p = tmp;
  }
  else
  {
    throw std::logic_error("Fail to read\n");
  }
  return in;
}

std::istream &abramov::operator>>(std::istream &in, Polygon &polygon)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  size_t k = 0;
  in >> k;
  if (k < 3)
  {
    throw std::logic_error("Wrong size\n");
  }
  std::copy_n(std::istream_iterator< Point >(in), k, std::back_inserter(polygon.points));
  return in;
}

double abramov::getArea(const Polygon &polygon)
{
  const std::vector< abramov::Point > &points = polygon.points;
  std::vector< int > sums;
  std::transform(points.cbegin(), points.cend() - 1, points.cbegin() + 1, std::back_inserter(sums), pointsSum);
  sums.push_back(pointsSum(points.back(), points.front()));
  return std::abs(std::accumulate(sums.begin(), sums.end(), 0)) / 2.0;
}

double abramov::areaEven(double s, const Polygon &polygon)
{
  if (isEven(polygon))
  {
    return s + getArea(polygon);
  }
  return s;
}

double abramov::areaOdd(double s, const Polygon &polygon)
{
  if (!isEven(polygon))
  {
    return s + getArea(polygon);
  }
  return s;
}

double abramov::areaMean(const std::vector< Polygon > &polygons)
{
  double s1 = std::accumulate(polygons.begin(), polygons.end(), 0.0, areaEven);
  double s2 = std::accumulate(polygons.begin(), polygons.end(), 0.0, areaOdd);
  return (s1 + s2) / polygons.size();
}

double abramov::areaVertexes(double s, const Polygon &polygon, size_t vert)
{
  if (polygon.points.size() == vert)
  {
    return s + getArea(polygon);
  }
  return s;
}

bool abramov::maxArea(const Polygon &p1, const Polygon &p2)
{
  return getArea(p1) < getArea(p2);
}

bool abramov::maxVertexes(const Polygon &p1, const Polygon &p2)
{
  return p1.points.size() < p2.points.size();
}
