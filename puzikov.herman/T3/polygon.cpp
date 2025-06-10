#include "polygon.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include <format_guard.hpp>
#include <input_wrapper_structs.hpp>

std::istream &puzikov::operator>>(std::istream &in, puzikov::Point &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> input::Character{'('} >> dest.x >> input::Character{';'} >> dest.y >> input::Character{')'};
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

std::istream &puzikov::operator>>(std::istream &in, Polygon &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t vertices = 0;
  if (!(in >> vertices) || vertices < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > temp(vertices);
  std::copy_n(std::istream_iterator< Point >(in), vertices, temp.begin());
  if (in)
  {
    dest.points = std::move(temp);
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
  FormatGuard guard(out);

  out << src.points.size() << ' ';
  std::copy(src.points.begin(), src.points.end(), std::ostream_iterator< Point >{out, " "});
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

double puzikov::calcPolygonArea(const puzikov::Polygon &poly)
{
  if (poly.points.size() < 3)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  double area = std::accumulate(poly.points.begin(), poly.points.end(), 0.0, PolygonAreaAccumulator(poly)) / 2.0;

  return std::abs(area);
}

void puzikov::readPolygons(std::istream &in, std::vector< Polygon > &polygons)
{
  using polygonInputIter = std::istream_iterator< Polygon >;
  while (!in.eof())
  {
    try
    {
      std::copy(polygonInputIter{in}, polygonInputIter{}, std::back_inserter(polygons));
    }
    catch (...)
    {
      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}

void puzikov::writePolygons(std::ostream &out, std::vector< Polygon > &vec)
{
  std::copy(vec.begin(), vec.end(), std::ostream_iterator< Polygon >(out, "\n"));
}
