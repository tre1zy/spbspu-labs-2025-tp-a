#include "geom.hpp"
#include <istream>
#include <ostream>
#include <iterator>
#include <vector>

std::istream& nikonov::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char open = '0';
  char sep = '0';
  char close = '0';
  if (!(in >> open) || open != '('
      || !(in >> point.x)
      || !(in >> sep) || sep != ';'
      || !(in >> point.y)
      || !(in >> close) || close != ')')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  return in;
}

std::istream& nikonov::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t cnt = 0;
  if (!(in >> cnt) || cnt < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > pts(cnt);
  for (size_t i = 0; i < cnt; ++i)
  {
    if (!(in >> pts[i]))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }
  polygon.points = std::move(pts);
  return in;
}

std::ostream& nikonov::operator<<(std::ostream& out, const Point& point)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  return out << '(' << point.x << ';' << point.y << ')';
}

std::ostream& nikonov::operator<<(std::ostream& out, const Polygon& polygon)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << polygon.points.size();
  for (size_t i = 0; i < polygon.points.size(); ++i)
  {
    out << ' ' << polygon.points[i];
  }
  return out;
}

bool nikonov::operator==(const Point& a, const Point& b)
{
  return (a.x == b.x && a.y == b.y);
}

bool nikonov::operator==(const Polygon& a, const Polygon& b)
{
  return a.points == b.points;
}
