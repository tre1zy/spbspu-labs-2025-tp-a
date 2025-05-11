#include "polygon.hpp"
#include <cmath>
#include <istream>
#include <iterator>
#include <vector>
#include <numeric>
#include "scopeGuard.hpp"
#include "delimeters.hpp"

namespace
{
  struct AreaCalculator
  {
    const kiselev::Point* point;
    double operator()(double area, const kiselev::Point& point2)
    {
      area += (point->x * point2.y) - (point2.x * point->y);
      point = &point2;
      return area;
    }
  };
}
std::istream& kiselev::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  detail::ScopeGuard scope(in);
  Point temp;
  in >> DelimeterIO{ '(' } >> temp.x >> DelimeterIO{ ';' } >> temp.y >> DelimeterIO{ ')' };
  if (in)
  {
    point = temp;
  }
  return in;
}

std::istream& kiselev::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  detail::ScopeGuard scope(in);
  size_t count = 0;
  in >> count;
  if (count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > temp;
  temp.reserve(count);
  std::copy(std::istream_iterator< Point >(in), std::istream_iterator< Point >(), std::back_inserter(temp));
  if (in)
  {
    polygon.points = std::move(temp);
  }
  return in;
}

double kiselev::getArea(const Polygon& polygon)
{
  AreaCalculator calc{ &polygon.points.back() };
  double area = std::accumulate(polygon.points.begin(), polygon.points.end(), 0.0, std::ref(calc));
  return std::abs(area) / 2.0;
}
