#include "GeometricalTypes.hpp"
#include <algorithm>
#include <iterator>
#include <limits>
#include <delimiter.hpp>

using check = shapkov::DelimiterIO;

std::istream& shapkov::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  int x = 0, y = 0;
  in >> check{ '(' };
  in >> x;
  in >> check{ ';' };
  in >> y;
  in >> check{ ')' };
  if (in)
  {
    point.x = x;
    point.y = y;
  }
  return in;
}

std::ostream& shapkov::operator<<(std::ostream& out, const Point& point)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  return (out << " (" << point.x << ';' << point.y << ')');
}

std::istream& shapkov::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  };
  int vertexes = 0;
  in >> vertexes;
  std::vector< Point > temp;
  std::copy_n(std::istream_iterator< Point >(in), vertexes, std::back_inserter(temp));
  if (in)
  {
    polygon.points = temp;
  }
  return in;
}

std::ostream& shapkov::operator<<(std::ostream& out, const Polygon& polygon)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  out << polygon.points.size();
  std::copy(std::begin(polygon.points), std::end(polygon.points), std::ostream_iterator< Point >(out, ""));
  return out;
}
