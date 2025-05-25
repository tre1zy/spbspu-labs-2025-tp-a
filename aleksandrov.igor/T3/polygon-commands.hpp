#ifndef POLYGON_COMMANDS_HPP
#define POLYGON_COMMANDS_HPP

#include <ios>
#include <vector>
#include "geometry.hpp"

namespace aleksandrov
{
  void getPolygons(std::istream&, std::vector< Polygon >&);
  void area(const std::vector< Polygon >&, std::ostream&);
  void max(const std::vector< Polygon >&, std::ostream&);
  void min(const std::vector< Polygon >&, std::ostream&);
  void count(const std::vector< Polygon >&, std::istream&, std::ostream&);
}

#endif

