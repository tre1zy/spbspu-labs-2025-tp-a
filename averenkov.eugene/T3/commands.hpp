#include <iostream>
#include <functional>
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <dataStruct.hpp>
#include "polygon.hpp"

namespace averenkov
{
  struct AreaAccumulator
  {
    double operator()(double sum, const Polygon& poly) const;
  };

  struct EvenAreaAccumulator
  {
    double operator()(double sum, const Polygon& poly) const;
  };

  struct OddAreaAccumulator
  {
    double operator()(double sum, const Polygon& poly) const;
  };

  struct NumVertexAreaAccumulator
  {
    size_t num;
    double operator()(double sum, const Polygon& poly) const;
  };

  struct AreaComparator
  {
    bool operator()(const Polygon& a, const Polygon& b) const;
  };

  struct VertexCountComparator
  {
    bool operator()(const Polygon& a, const Polygon& b) const;
  };

  struct EvenVertexCounter
  {
    bool operator()(const Polygon& poly) const;
  };

  struct OddVertexCounter
  {
    bool operator()(const Polygon& poly) const;
  };

  struct NumVertexCounter
  {
    size_t num;
    bool operator()(const Polygon& poly) const;
  };

  struct RightAngleChecker
  {
    bool operator()(const Polygon& poly) const;
  };

  struct PolygonEqual
  {
    const Polygon& target;
    bool operator()(const Polygon& a, const Polygon& b) const;
  };

  double calculateArea(const Polygon& poly);
  void printAreaSum(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out);
  void printMaxValueOf(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out);
  void printMinValueOf(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out);
  void printCountOf(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out);
  void printPermsCnt(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out);
  void printRightsCnt(const std::vector< Polygon >& polygons, std::ostream& out);
  void printRmEcho(std::istream& in, std::vector< Polygon >& polygons, std::ostream& out);
}
