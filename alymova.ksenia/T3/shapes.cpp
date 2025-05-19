#include "shapes.hpp"
#include <algorithm>
#include <iterator>
#include <functional>
#include <string>

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
  using namespace std::placeholders;

  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  size_t cnt_points;
  in >> cnt_points;
  if (!in || cnt_points < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > tested;
  auto it_find = std::find_if(
    std::istream_iterator< Point >(in),
    std::istream_iterator< Point >(),
    std::bind(insertIfNotNextEnter, std::ref(in), _1, std::ref(tested))
  );
  if (in)
  {
    tested.push_back(*it_find);
  }
  if (tested.size() == cnt_points)
  {
    polygon.points = std::move(tested);
  }
  else
  {
    in.setstate(std::ios::failbit);
  }
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

bool alymova::insertIfNotNextEnter(std::istream& in, const Point& point, std::vector< Point >& dop)
{
  in >> std::noskipws;
  char c = in.peek();
  in >> std::skipws;

  if (c != '\n')
  {
    dop.push_back(point);
    return false;
  }
  return true;
}
