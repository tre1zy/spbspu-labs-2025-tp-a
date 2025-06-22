#ifndef FUNCTIONAL_HPP
#define FUNCTIONAL_HPP

#include <algorithm>
#include <functional>
#include <string>
#include <vector>

#include "polygon.hpp"

namespace fedorov
{
  struct PolygonValidator
  {
    bool operator()(const Polygon &poly) const;
  };

  struct AreaComparator
  {
    bool operator()(const Polygon &p1, const Polygon &p2) const;
  };

  struct VerticesComparator
  {
    bool operator()(const Polygon &p1, const Polygon &p2) const;
  };

  struct AreaOddAccumulator
  {
    double operator()(double acc, const Polygon &poly) const;
  };

  struct AreaEvenAccumulator
  {
    double operator()(double acc, const Polygon &poly) const;
  };

  struct AreaMeanAccumulator
  {
    double operator()(double acc, const Polygon &poly) const;
  };

  struct AreaNumAccumulator
  {
    explicit AreaNumAccumulator(size_t num);
    double operator()(double acc, const Polygon &poly) const;
    size_t num;
  };

  struct CountOddPredicate
  {
    bool operator()(const Polygon &poly) const;
  };

  struct CountEvenPredicate
  {
    bool operator()(const Polygon &poly) const;
  };

  struct CountNumPredicate
  {
    explicit CountNumPredicate(size_t num);
    bool operator()(const Polygon &poly) const;
    size_t num;
  };

  struct LessAreaPredicate
  {
    explicit LessAreaPredicate(double threshold);
    bool operator()(const Polygon &poly) const;
    double threshold;
  };

  double calcPolygonArea(const Polygon &poly);
  void readPolygons(std::istream &in, std::vector< Polygon > &polygons);
}

#endif
