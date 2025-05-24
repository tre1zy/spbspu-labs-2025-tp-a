#ifndef FUNCTORS_HPP
#define FUNCTORS_HPP

#include <cstddef>
#include "polygon.hpp"

namespace trukhanov
{
  struct isSize
  {
    size_t size;
    bool operator()(const Polygon& p) const
    {
      return p.points.size() == size;
    }
  };

  struct CompareByArea
  {
    bool operator()(const Polygon& lhs, const Polygon& rhs) const;
  };

  struct LessArea
  {
    double area;
    bool operator()(const Polygon& p) const;
  };

  struct AccumulateAll
  {
    double operator()(double sum, const Polygon& p) const;
  };

  struct CompareByVertexes
  {
    bool operator()(const Polygon& lhs, const Polygon& rhs) const;
  };

  struct HasDuplicates
  {
    bool operator()(const Polygon& p) const;
  };

  struct IsEvenAndValid
  {
    bool operator()(const Polygon& p) const;
  };

  struct IsOddAndValid
  {
    bool operator()(const Polygon& p) const;
  };
}

#endif
