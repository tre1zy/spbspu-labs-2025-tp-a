#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <vector>
#include <iterator>
#include <istream>
#include <algorithm>
#include <dataStruct.hpp>

namespace averenkov
{
  struct Point
  {
    int x;
    int y;
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
  };

  std::istream& operator>>(std::istream& in, Point& p);

  struct Polygon
  {
    std::vector< Point > points;
    bool operator==(const Polygon& other) const;
  };

  std::istream& operator>>(std::istream& in, Polygon& poly);

};


bool averenkov::Point::operator==(const Point& other) const
{
  return x == other.x && y == other.y;
}

bool averenkov::Point::operator!=(const Point& other) const
{
  return !(*this == other);
}

std::istream& averenkov::operator>>(std::istream& in, Point& p)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> averenkov::DelimiterIO{'('};
  in >> p.x;
  in >> averenkov::DelimiterIO{';'};
  in >> p.y;
  in >> averenkov::DelimiterIO{')'};
  return in;
}

std::istream& averenkov::operator>>(std::istream& in, averenkov::Polygon& poly)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t numPoints = 0;
  in >> numPoints;
  if (numPoints < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > points(numPoints);
  poly.points.resize(numPoints);
  std::copy_n(std::istream_iterator< Point >(in), numPoints, points.begin());
  if (poly.points.size() != numPoints)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  poly.points = std::move(points);
  return in;
}

bool averenkov::Polygon::operator==(const Polygon& other) const
{
  return points == other.points;
}

#endif
