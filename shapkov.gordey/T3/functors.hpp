#ifndef FUNCTORS_HPP
#define FUNCTORS_HPP
#include "GeometricalTypes.hpp"

namespace shapkov
{
  struct HasSize
  {
    size_t size;
    bool operator()(const Polygon& p) const {
      return p.points.size() == size;
    }
  };
}

#endif
