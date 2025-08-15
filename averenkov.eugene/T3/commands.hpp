#include <iostream>
#include <functional>
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <map>
#include <dataStruct.hpp>
#include "polygon.hpp"

namespace averenkov
{
  struct Prod
  {
    double operator()(const averenkov::Point& a, const averenkov::Point& b) const;
  };

  struct VertexCount
  {
    VertexCount(size_t num);
    bool operator()(const Polygon& poly) const;

  private:
    size_t num_;
  };

  struct AreaComparator
  {
    bool operator()(const Polygon& a, const Polygon& b) const;
  };

  struct VertexCountComparator
  {
    bool operator()(const Polygon& a, const Polygon& b) const;
  };

  struct AreaSumCalculator
  {
    double operator()(const std::vector< Polygon >& polygons) const;
  private:
    double calculateRecursive(const std::vector<Polygon>& polygons, size_t index, double sum) const;
  };

  struct EvenSumCalculator
  {
    double operator()(const std::vector< Polygon >& polygons) const;
  private:
    double calculateRecursive(const std::vector<Polygon>& polygons, size_t index, double sum) const;
  };

  struct OddSumCalculator
  {
    double operator()(const std::vector< Polygon >& polygons) const;
  private:
    double calculateRecursive(const std::vector<Polygon>& polygons, size_t index, double sum) const;
  };

  struct IsEven
  {
    bool operator()(const Polygon& poly) const;
  };

  struct IsOdd
  {
    bool operator()(const Polygon& poly) const;
  };

  struct NumVertexCalculator
  {
    size_t num;
    double operator()(const std::vector< Polygon >& polygons) const;
  private:
    double calculateRecursive(const std::vector<Polygon>& polygons, size_t index, double sum) const;
 };

  struct MeanAreaCalculator
  {
    double operator()(const std::vector< Polygon >& polygons) const;
  };

  struct MaxAreaFinder
  {
    void operator()(const std::vector< Polygon >& polygons, std::ostream& out) const;
  };

  struct MaxVertexCountFinder
  {
    void operator()(const std::vector< Polygon >& polygons, std::ostream& out) const;
  };

  struct MinAreaFinder
  {
    void operator()(const std::vector< Polygon >& polygons, std::ostream& out) const;
  };

  struct MinVertexCountFinder
  {
    void operator()(const std::vector< Polygon >& polygons, std::ostream& out) const;
  };

  struct EvenCounter
  {
    size_t operator()(const std::vector< Polygon >& polygons) const;
  };

  struct OddCounter
  {
    size_t operator()(const std::vector< Polygon >& polygons) const;
  };

  struct NumVertexCounter
  {
    size_t num;
    size_t operator()(const std::vector< Polygon >& polygons) const;
  };

  struct AngleCheckHelper
  {
    const std::vector< Point >& pts;
    size_t idx;
    AngleCheckHelper(const std::vector< Point >& points);
    bool operator()(const Point&);
  };

  struct RightAngleChecker
  {
    bool operator()(const Polygon& poly) const;
  };

  struct PolygonEqual
  {
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
