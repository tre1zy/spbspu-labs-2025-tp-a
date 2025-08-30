#include "geometry.hpp"
#include <algorithm>
#include <functional>
#include <iterator>
#include <delimeterIO.hpp>

std::istream& krylov::operator>>(std::istream& in, Point& p)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  Point temp{ 0, 0 };
  in >> DelimiterIO{ '(' };
  in >> temp.x;
  in >> DelimiterIO{ ';' };
  in >> temp.y;
  in >> DelimiterIO{ ')' };
  if (in)
  {
    p = temp;
  }
  return in;
}

std::istream& krylov::operator>>(std::istream& in, Polygon& polygon)
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
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > points(k);
  std::copy_n(std::istream_iterator< Point >(in), k, points.begin());
  if (!in)
  {
    return in;
  }
  polygon.points = points;
  return in;
}


