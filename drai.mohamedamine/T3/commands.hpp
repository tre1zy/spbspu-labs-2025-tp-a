#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "polygon.hpp"
#include <vector>

namespace amine
{
  void process_commands(std::vector< Polygon >& polygons);
  void process_rmecho(std::vector< Polygon >& polygons, const Polygon& query);
  int countIntersections(const std::vector< Polygon >& polygons, const Polygon& query);
  bool isPolygonEqual(const Polygon& a, const Polygon& b);
}

#endif
