#include "polygon.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include "input_delim.hpp"
#include "format_guard.hpp"

std::istream &puzikov::operator>>(std::istream &in, puzikov::Point &dest)
{
  std::istream::sentry sentry(in);
  if (sentry)
  {
    using delim = puzikov::DelimiterI;
    in >> delim {'('} >> dest.x >> delim {';'} >> dest.y >> delim {')'};
  }
  return in;
}

std::ostream &puzikov::operator<<(std::ostream &out, const puzikov::Point &src)
{
  std::ostream::sentry sentry(out);
  if (sentry)
  {
    puzikov::FormatGuard guard(out);
    out << '(' << src.x << ';' << src.y << ')';
  }
  return out;
}

std::istream &puzikov::operator>>(std::istream &in, puzikov::Polygon &dest)
{
  std::istream::sentry sentry(in);
  if (sentry)
  {
    int n;
    if (in >> n)
    {
      dest.points.clear();
      std::copy_n(std::istream_iterator< puzikov::Point > {in}, n, std::back_inserter(dest.points));
    }
  }
  return in;
}

std::ostream &puzikov::operator<<(std::ostream &out, const puzikov::Polygon &src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  puzikov::FormatGuard guard(out);
  using output_it_t = std::ostream_iterator< puzikov::Point >;

  out << src.points.size() << ' ';
  std::copy(src.points.begin(), src.points.end(), output_it_t {out, " "});
  return out;
}

bool puzikov::operator==(const Point &p1, const Point &p2)
{
  return ((p1.x == p2.x) && (p1.y == p2.y));
}

bool puzikov::operator==(const Polygon &p1, const Polygon &p2)
{
  if (p1.points.size() != p2.points.size())
  {
    return false;
  }
  return std::equal(p1.points.begin(), p1.points.end(), p2.points.begin());
}

bool puzikov::operator!=(const Polygon &p1, const Polygon &p2)
{
  return !(p1 == p2);
}

struct puzikov::PolygonAreaAccumulator
{
  PolygonAreaAccumulator(const puzikov::Polygon &p):
    poly(p),
    n(p.points.size())
  {}

  double operator()(double acc, const puzikov::Point &p1) const
  {
    size_t i = &p1 - &poly.points[0];
    const puzikov::Point &p2 = poly.points[(i + 1) % n];
    return acc + (p1.x * p2.y - p2.x * p1.y);
  }
  const puzikov::Polygon &poly;
  size_t n;
};

// Shoelace polygon area algo
double puzikov::calcPolygonArea(const puzikov::Polygon &poly)
{
  if (poly.points.size() < 3)
  {
    return 0.0;
  }
  double area = std::accumulate(poly.points.begin(), poly.points.end(), 0.0, PolygonAreaAccumulator(poly)) / 2.0;

  // need an absolute value because the sign of the area depends on whether the points are
  // defined clock-wise (the area would be negative) or counter-clock-wise (the area would be positive)
  return std::abs(area);
}

void puzikov::readPolygons(std::istream &in, std::vector< Polygon > &polygons)
{
  using input_it_t = std::istream_iterator< Polygon >;
  while (!in.eof())
  {
    std::copy(input_it_t(in), input_it_t(), std::back_inserter(polygons));
    if (in.fail())
    {
      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
