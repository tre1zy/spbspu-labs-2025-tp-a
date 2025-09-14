#ifndef COMMAND_HANDLING_HPP
#define COMMAND_HANDLING_HPP

#include <iostream>
#include "polygon.hpp"

namespace holodilov::commands
{
  void echo(std::istream& is, std::ostream& os, std::vector< Polygon >& vecPolygons);

  void lessArea(std::istream& is, std::ostream& os, const std::vector< Polygon >& vecPolygons);
}

#endif