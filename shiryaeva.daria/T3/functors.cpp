#include "functors.hpp"

namespace shiryaeva
{
bool IsEven::operator()(const Polygon& polygon) const
{
  return polygon.points.size() % 2 == 0;
}

bool IsOdd::operator()(const Polygon& polygon) const
{
  return polygon.points.size() % 2 != 0;
}

bool HasVertexCount::operator()(const Polygon& polygon) const
{
  return polygon.points.size() == count;
}
}
