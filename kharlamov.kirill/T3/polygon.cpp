#include "polygon.h"
#include <algorithm>

namespace kharlamov
{
  bool Polygon::operator==(const Polygon& other) const
  {
    return points == other.points;
  }

  std::istream& operator>>(std::istream& in, Polygon& poly)
  {
    size_t vertexes;
    in >> vertexes;
    if (vertexes < 3)
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    poly.points.resize(vertexes);
    for (auto& point : poly.points)
    {
      in >> point;
    }
    return in;
  }
}
