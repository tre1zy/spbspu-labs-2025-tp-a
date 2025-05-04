#include "functors.hpp"
#include <cmath>

bool shapkov::HasSize::operator()(const Polygon& p) const {
  return p.points.size() == size;
}

bool shapkov::compareDouble(double d1, double d2, double epsilon)
{
  return std::abs(d1 - d2) < epsilon;
}
