#include "shapes.hpp"
#include <algorithm>
#include <iterator>

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
  size_t cnt_points;
  in >> cnt_points;
  if (cnt_points < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  //polygon.points.clear();
  Polygon tested;
  std::copy_n(std::istream_iterator< Point >(in), cnt_points, std::back_inserter(tested.points));
  /*for (size_t i = 0; i < cnt_points; i++)
  {
    if (in.peek() == '\n')
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    Point point;
    in >> DelimiterIO{' '} >> point;
    polygon.points.push_back(point);
    if (in.fail() || in.eof())
    {
      return in;
    }
  }*/
  if (in)
  {
    polygon = std::move(tested);
  }
  in >> std::noskipws;
  in >> DelimiterIO{'\n'};
  in >> std::skipws;
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
