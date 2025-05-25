#ifndef FUNCTORS_HPP
#define FUNCTORS_HPP

#include "polygon.hpp"

#include <algorithm>
#include <numeric>

constexpr size_t RECTANGLE_SIDES = 4;

namespace filonova
{
  struct IsOdd
  {
    bool operator()(const Polygon &p) const;
  };

  struct IsEven
  {
    bool operator()(const Polygon &p) const;
  };

  struct ComputeTotalArea
  {
    double operator()(const std::vector< Polygon > &polygons) const;
  };

  struct ShoelaceTermCalculator
  {
    double operator()(const Point &p1, const Point &p2) const;
  };

  struct HasVertexCount
  {
    size_t count_;
    HasVertexCount(size_t count);
    bool operator()(const Polygon &p) const;
  };

  struct CompareByArea
  {
    bool operator()(const Polygon &a, const Polygon &b) const;
  };

  struct CompareByVertexes
  {
    bool operator()(const Polygon &a, const Polygon &b) const;
  };

  struct IntersectsWith
  {
    Polygon polygon_;
    IntersectsWith(const Polygon &polygon);
    bool operator()(const Polygon &other) const;
  };

  struct IsRectangle
  {
    static double dot(const Point &a, const Point &b, const Point &c);
    static double getDistanceSquared(const Point &a, const Point &b);
    bool operator()(const Polygon &polygon) const;
  };
}

#endif
