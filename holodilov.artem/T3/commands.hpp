#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include "polygon.hpp"

namespace holodilov::commands
{
  void area(std::istream& is, std::ostream& os, const std::vector< Polygon >& vecPolygons);

  void min(std::istream& is, std::ostream& os, const std::vector< Polygon >& vecPolygons);

  void max(std::istream& is, std::ostream& os, const std::vector< Polygon >& vecPolygons);

  void count(std::istream& is, std::ostream& os, const std::vector< Polygon >& vecPolygons);

  void echo(std::istream& is, std::ostream& os, std::vector< Polygon >& vecPolygons);

  void lessArea(std::istream& is, std::ostream& os, const std::vector< Polygon >& vecPolygons);
}

#endif
