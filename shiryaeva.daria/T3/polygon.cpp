#include "polygon.hpp"
#include "input_struct.hpp"
#include <algorithm>
#include <cmath>

namespace shiryaeva
{
std::istream& operator>>(std::istream& in, Point& point)
{
  Point temp{0, 0};
  in >> DelimiterIO{'('} >> temp.x >> DelimiterIO{';'} >> temp.y >> DelimiterIO{')'};
  if (in)
  {
    point = temp;
  }
  return in;
}

std::istream& operator>>(std::istream& in, Polygon& polygon)
{
  size_t vertexCount = 0;
  in >> vertexCount;
  
  if (!in || vertexCount < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::vector< Point > points(vertexCount);
  for (auto& p : points)
  {
    if (!(in >> p))
    {
      return in;
    }
  }

  polygon.points = std::move(points);
  return in;
}
double getArea(const Polygon& polygon)
{
  double area = 0.0;
  const auto& points = polygon.points;
  size_t n = points.size();

  for (size_t i = 0; i < n; ++i)
  {
    const auto& p1 = points[i];
    const auto& p2 = points[(i + 1) % n];
    area += (p1.x * p2.y) - (p2.x * p1.y);
  }

  return std::abs(area) / 2.0;
}
}
