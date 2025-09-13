#include <functional>
#include <algorithm>
#include <delimiter_io.hpp>
#include "shapes.hpp"

std::istream& cherepkov::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sent(in);
  if (!sent)
  {
    return in;
  }

  Point tmp;
  in >> DelimiterIO{'('} >> tmp.x >> DelimiterIO{';'} >> tmp.y >> DelimiterIO{')'};

  if (in)
  {
    point = tmp;
  }
  return in;
}

std::istream& cherepkov::operator>>(std::istream& in, Polygon& polygon)
{
  using namespace std::placeholders;
  using input_it = std::istream_iterator< Point >;

  std::istream::sentry sent(in);
  if (!sent)
  {
    return in;
  }

  size_t vertex_count;
  in >> vertex_count;
  if (!in || vertex_count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::vector< Point > points;
  auto reader = std::bind(handleNewLine, std::ref(in), _1);

  std::transform(input_it{in}, input_it{}, std::back_inserter(points), reader);

  if (points.size() == vertex_count && in.eof())
  {
    in.clear();
    polygon.points = std::move(points);
  }
  else
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

cherepkov::Point cherepkov::handleNewLine(std::istream& in, const Point& point)
{
  if (in.peek() == '\n')
  {
    in.setstate(std::ios::eofbit);
  }
  return point;
}
