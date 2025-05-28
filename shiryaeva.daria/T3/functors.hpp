#ifndef FUNCTORS_HPP
#define FUNCTORS_HPP

#include "polygon.hpp"
#include <functional>

namespace shiryaeva
{
struct IsEven
{
  bool operator()(const Polygon& polygon) const;
};

struct IsOdd
{
  bool operator()(const Polygon& polygon) const;
};

struct HasVertexCount
{
  size_t count;
  bool operator()(const Polygon& polygon) const;
};
}

#endif
