#ifndef USER_COMMANDS_HPP
#define USER_COMMANDS_HPP
#include <iostream>
#include <iomanip>
#include <functional>
#include <numeric>
#include <map>
#include <stream-guard.hpp>
#include "shapes.hpp"
#include "sub-utils.hpp"

namespace alymova
{
  using namespace std::placeholders;
  using AreaSubcommands = std::map< std::string, std::function< double(double, const Polygon&) > >;
  using CountSubcommands = std::map< std::string, std::function< bool(const Polygon&) > >;
  using CmpArea = std::function< double(double, const Polygon&) >;
  using CmpVertexes = std::function< size_t(size_t, const Polygon&) >;

  void area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  template< class CmpArea, class CmpVertexes >
  void maxAndMin(CmpArea cmp_area, CmpVertexes cmp_vertexes,
    std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void inFrame(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void rightShapes(std::ostream& out, const std::vector< Polygon >& polygons);
}

template< class CmpArea, class CmpVertexes >
void alymova::maxAndMin(CmpArea cmp_area, CmpVertexes cmp_vertexes,
  std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  if (polygons.empty())
  {
    throw std::logic_error("");
  }
  std::string command;
  in >> command;
  if (command == "AREA")
  {
    double res = std::accumulate(polygons.begin(), polygons.end(), areaPolygon(polygons[0]), cmp_area);
    StreamGuard guard(out);
    out << std::fixed << std::setprecision(1) << res;
    return;
  }
  if (command == "VERTEXES")
  {
    out << std::accumulate(polygons.begin(), polygons.end(), polygons[0].points.size(), cmp_vertexes);
    return;
  }
  throw std::logic_error("");
}
#endif
