#include "shapes.hpp"
#include <iterator>
#include <algorithm>

std::istream& alymova::operator>>(std::istream& in, DelimiterIO&& object)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  char delim;
  if (in >> delim && delim != object.exp)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& alymova::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  in >> DelimiterIO{'('} >> point.x >> DelimiterIO{';'} >> point.y >> DelimiterIO{')'};
  return in;
}

std::istream& alymova::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  polygon.points.clear();
  size_t cnt_points;
  in >> cnt_points;
  std::copy_n(
    std::istream_iterator< Point >(in),
    cnt_points,
    std::back_inserter(polygon.points)
  );
  return in;
}

std::ostream& alymova::operator<<(std::ostream& out, const Point& point)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  std::cout << '(' << point.x << ';' << point.y << ')';
  return out;
}

std::ostream& alymova::operator<<(std::ostream& out, const Polygon& polygon)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  std::copy_n(
    polygon.points.begin(),
    polygon.points.size() - 1,
    std::ostream_iterator< Point >(out, " ")
  );
  out << polygon.points[polygon.points.size() - 1];
  return out;
}
