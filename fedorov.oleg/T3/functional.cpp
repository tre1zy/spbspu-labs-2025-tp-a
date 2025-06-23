#include "functional.hpp"

#include <iterator>
#include <limits>
#include <numeric>
#include <cmath>
#include <functional>
#include "polygon.hpp"

namespace fedorov
{
  struct PointArea
  {
    double operator()(const Point &a, const Point &b) const
    {
      return a.x * b.y - b.x * a.y;
    }
  };

  double calcPolygonArea(const Polygon &poly)
  {
    if (poly.points.size() < 3)
    {
      return 0.0;
    }

    double sum = 0.0;
    const Point &first = poly.points.front();
    const Point &last = poly.points.back();

    if (poly.points.size() > 1)
    {
      auto begin = poly.points.begin();
      auto end = poly.points.end();
      sum = std::inner_product(begin, end - 1, begin + 1, 0.0, std::plus<>{}, PointArea{});
    }

    sum += (last.x * first.y - first.x * last.y);
    return std::abs(sum) / 2.0;
  }

  bool PolygonValidator::operator()(const Polygon &poly) const
  {
    return poly.points.size() >= 3;
  }

  void readPolygons(std::istream &in, std::vector< Polygon > &polygons)
  {
    using inputIt = std::istream_iterator< Polygon >;
    while (!in.eof())
    {
      if (in.fail())
      {
        in.clear(in.rdstate() ^ std::ios::failbit);
        in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
      std::copy(inputIt(in), inputIt(), std::back_inserter(polygons));
    }
  }

  bool AreaComparator::operator()(const Polygon &p1, const Polygon &p2) const
  {
    return calcPolygonArea(p1) < calcPolygonArea(p2);
  }

  bool VerticesComparator::operator()(const Polygon &p1, const Polygon &p2) const
  {
    return p1.points.size() < p2.points.size();
  }

  double AreaOddAccumulator::operator()(double acc, const Polygon &poly) const
  {
    return (poly.points.size() % 2 != 0) ? acc + calcPolygonArea(poly) : acc;
  }

  double AreaEvenAccumulator::operator()(double acc, const Polygon &poly) const
  {
    return (poly.points.size() % 2 == 0) ? acc + calcPolygonArea(poly) : acc;
  }

  double AreaMeanAccumulator::operator()(double acc, const Polygon &poly) const
  {
    return acc + calcPolygonArea(poly);
  }

  AreaNumAccumulator::AreaNumAccumulator(size_t num):
    num(num)
  {}

  double AreaNumAccumulator::operator()(double acc, const Polygon &poly) const
  {
    return (poly.points.size() == num) ? acc + calcPolygonArea(poly) : acc;
  }

  bool CountOddPredicate::operator()(const Polygon &poly) const
  {
    return poly.points.size() % 2 != 0;
  }

  bool CountEvenPredicate::operator()(const Polygon &poly) const
  {
    return poly.points.size() % 2 == 0;
  }

  CountNumPredicate::CountNumPredicate(size_t num):
    num(num)
  {}

  bool CountNumPredicate::operator()(const Polygon &poly) const
  {
    return poly.points.size() == num;
  }

  LessAreaPredicate::LessAreaPredicate(double threshold):
    threshold(threshold)
  {}

  bool LessAreaPredicate::operator()(const Polygon &poly) const
  {
    return calcPolygonArea(poly) < threshold;
  }
}
