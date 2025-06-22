#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <vector>
#include "polygon.hpp"

namespace fedorov
{
  void maxArea(std::ostream &out, const std::vector< Polygon > &polys);
  void minArea(std::ostream &out, const std::vector< Polygon > &polys);
  void maxVertices(std::ostream &out, const std::vector< Polygon > &polys);
  void minVertices(std::ostream &out, const std::vector< Polygon > &polys);

  void areaCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void lessAreaCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void maxCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polys);
  void minCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polys);
  void echoCommand(std::istream &in, std::ostream &out, std::vector< Polygon > &polygons);
  void countCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
}

#endif
