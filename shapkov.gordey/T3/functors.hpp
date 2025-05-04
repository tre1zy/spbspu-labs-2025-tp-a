#ifndef FUNCTORS_HPP
#define FUNCTORS_HPP
#include "GeometricalTypes.hpp"

namespace shapkov
{
  struct HasSize
  {
    size_t size;
    bool operator()(const Polygon& p) const;
  };
  bool compareDouble(double d1, double d2, double epsilon = 1e-9);
}

#endif
