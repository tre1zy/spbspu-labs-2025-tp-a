#include "polygon.hpp"
#include "functors.hpp"
#include "input_struct.hpp"

std::istream &filonova::operator>>(std::istream &in, Point &point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  Point temp{0, 0};
  in >> DelimiterValue{'('} >> temp.x >> DelimiterValue{';'} >> temp.y >> DelimiterValue{')'};

  if (in)
  {
    point = temp;
  }
  return in;
}

bool filonova::operator<(const Point &lhs, const Point &rhs)
{
  return (lhs.x < rhs.x) && (lhs.y < rhs.y);
}

std::istream &filonova::operator>>(std::istream &in, Polygon &polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  size_t vertexCount;
  in >> vertexCount;
  if (!in || vertexCount < MIN_VERTEX_COUNT)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::vector< Point > points;
  points.reserve(vertexCount);

  for (size_t i = 0; i < vertexCount; ++i)
  {
    Point point;
    if (!(in >> point))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    points.push_back(point);
  }

  polygon.points = std::move(points);
  return in;
}

double filonova::getArea(const Polygon &polygon)
{
  const std::vector< Point > &pts = polygon.points;
  if (pts.size() < MIN_VERTEX_COUNT)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  std::vector< Point > temp_pts = pts;
  temp_pts.push_back(pts[0]);

  double sum_terms = std::transform_reduce(
      temp_pts.begin(),
      temp_pts.end() - 1,
      temp_pts.begin() + 1,
      0.0,
      std::plus< double >(),
      filonova::ShoelaceTermCalculator());

  return std::abs(sum_terms) / 2.0;
}
