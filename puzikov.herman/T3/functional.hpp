#ifndef FUNCTIONAL_HPP
#define FUNCTIONAL_HPP

#include <vector>
#include <string>
#include "polygon.hpp"

namespace puzikov
{
  struct AreaAccumulator
  {
    AreaAccumulator(const std::string &p);
    double operator()(double acc, const puzikov::Polygon &poly) const;
    const std::string &param;
  };

  bool VerticesComparator(const Polygon &p1, const Polygon &p2);
  bool AreaComparator(const Polygon &p1, const Polygon &p2);

  struct ShapesAccumulator
  {
    ShapesAccumulator(const std::string &p);
    double operator()(double acc, const puzikov::Polygon &poly) const;
    const std::string &param;
  };

  struct RmEchoPredicate
  {
    RmEchoPredicate(const Polygon &poly);
    bool operator()(const Polygon &p1, const Polygon &p2);
    const Polygon &ref;
  };

  struct TranslatePoint
  {
    TranslatePoint(int dx, int dy);
    Point operator()(const Point &p) const;
    int dx_, dy_;
  };

  struct AnyOfShift
  {
    using PointVec = std::vector< Point >;
    AnyOfShift(const PointVec &candidate_, const PointVec &reference_);
    bool operator()(std::size_t shift) const;
    const PointVec &candidate;
    const PointVec &reference;
  };

  struct IsTranslationCongruent
  {
    IsTranslationCongruent(const Polygon &reference_);
    bool operator()(const Polygon &poly) const;
    const Polygon &reference;
  };

  using AreaComp = bool (*)(const Polygon &, const Polygon &);
  using VertComp = bool (*)(const Polygon &, const Polygon &);
  using constPolygonVecIt = std::vector< Polygon >::const_iterator;
  using AreaAlgo = constPolygonVecIt (*)(constPolygonVecIt, constPolygonVecIt, AreaComp);
  using VertAlgo = constPolygonVecIt (*)(constPolygonVecIt, constPolygonVecIt, VertComp);

  constPolygonVecIt maxAreaElement(constPolygonVecIt first, constPolygonVecIt last, AreaComp comp);
  constPolygonVecIt minAreaElement(constPolygonVecIt first, constPolygonVecIt last, AreaComp comp);
  constPolygonVecIt maxVertElement(constPolygonVecIt first, constPolygonVecIt last, VertComp comp);
  constPolygonVecIt minVertElement(constPolygonVecIt first, constPolygonVecIt last, VertComp comp);
}

#endif
