#include <algorithm>
#include <numeric>
#include <cmath>
#include <iterator>
#include "geometry.hpp"
#include "delimiter.hpp"

namespace
{
  using namespace geom;

  double crossProduct(const Point& a, const Point& b)
  {
    return static_cast< double >(a.x) * b.y - static_cast< double >(b.x) * a.y;
  }
}

bool geom::operator==(const Point& p1, const Point& p2)
{
  return p1.x == p2.x && p1.y == p2.y;
}

std::istream& geom::operator>>(std::istream& in, Point& p)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  using del = io::DelimiterIO;
  return in >> del{ '(' } >> p.x >> del{ ';' } >> p.y >> del{ ')' };
}

std::istream& geom::operator>>(std::istream& in, Polygon& poly)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  size_t count = 0;
  if (!(in >> count) || count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::vector< Point > pts(count);
  auto it = std::istream_iterator< Point >(in);
  std::copy_n(it, count, pts.begin());
  if (!in)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  poly.points = std::move(pts);
  return in;
}

double geom::getPolygonArea(const Polygon& poly)
{
  const auto& pts = poly.points;
  if (pts.size() < 3)
  {
    return 0;
  }
  std::vector< Point > shifted(pts.begin() + 1, pts.end());
  shifted.push_back(pts.front());

  double area = std::inner_product(
    pts.begin(), pts.end(),
    shifted.begin(),
    0.0,
    std::plus<>(),
    crossProduct
  );

  return std::abs(area) / 2.0;
}
