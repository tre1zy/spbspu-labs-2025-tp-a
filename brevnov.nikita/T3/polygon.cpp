#include "polygon.hpp"
#include <algorithm>
#include <cmath>
#include <istream>
#include <iterator>
#include <vector>
#include <numeric>
#include <functional>
#include "streamGuard.hpp"
#include "delimeters.hpp"

namespace
{
  double calc_area_term(const brevnov::Point& p1, const brevnov::Point& p2)
  {
    return p1.x * p2.y - p2.x * p1.y;
  }

  double distance(const brevnov::Point p1, const brevnov::Point& p2)
  {
    return std::sqrt(std::pow((p1.x - p2.x), 2) + std::pow((p1.y - p2.y), 2));
  }
}

std::istream& brevnov::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sent(in);
  if (!sent)
  {
    return in;
  }
  brevnov::streamGuard stream(in);
  in >> DelimeterIO{ '(' } >> point.x >> DelimeterIO{ ';' } >> point.y >> DelimeterIO{ ')' };
  return in;
}


std::istream& brevnov::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sent(in);
  if (!sent)
  {
    return in;
  }
  brevnov::streamGuard stream(in);
  size_t count = 0;
  if (!(in >> count) || count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > temp(count);
  std::copy_n(std::istream_iterator< Point >(in), count, temp.begin());
  if (in)
  {
    polygon.points = std::move(temp);
  }
  return in;
}

double brevnov::get_area(const Polygon& polygon)
{
  const auto points = polygon.points;
  const Point first = points.front();
  const Point last = points.back();
  double area = std::inner_product(
    points.begin(), points.end() - 1, points.begin() + 1, CalcAreaTerm()(last, first), std::plus< double >(), CalcAreaTerm());
  return std::abs(area) / 2.0;
}
