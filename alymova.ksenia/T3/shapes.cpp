#include "shapes.hpp"
#include <algorithm>
#include <iterator>
#include <functional>
#include <string>
#include <delimiter-io.hpp>

std::istream& alymova::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  Point tested;
  in >> DelimiterIO{'('} >> tested.x >> DelimiterIO{';'} >> tested.y >> DelimiterIO{')'};
  if (in)
  {
    point = tested;
  }
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
  /*auto first = std::istream_iterator< Point >(in);
  auto result = std::back_inserter(tested);
  *result = *first;
  ++result;
  for (size_t i = 1; i != cnt_points; ++i, (void)++result)
  {
    std::cout << *first << '\n';
    *result = *++first;
  }*/
  //std::copy(std::istream_iterator< Point >(in), std::istream_iterator< Point >(), std::back_inserter(tested));
  auto it = std::istream_iterator< Point >(in);
  std::transform(
    it,
    std::istream_iterator< Point >(),
    std::back_inserter(tested),
    std::bind(checkNextEnter, std::ref(in), _1)
  );
  //std::cout << tested.size() << '\n';
  /*for (size_t i = 0; i < tested.size(); i++)
  {
    std::cout << tested[i] << '\n';
  }*/
  /*auto it_find = std::find_if(
    std::istream_iterator< Point >(in),
    std::istream_iterator< Point >(),
    std::bind(insertIfNotNextEnter, std::ref(in), _1, std::ref(tested))
  );
  if (in)
  {
    tested.push_back(*it_find);
  }*/
  if (tested.size() == cnt_points)
  {
    in.clear(in.rdstate() ^ std::ios_base::failbit);
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

alymova::Point alymova::checkNextEnter(std::istream& in, const Point& point)
{
  char c = in.peek();
  if (c == '\n')
  {
    in.setstate(std::ios::failbit);
  }
  return point;
}
