#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include "polygon.hpp"
#include <iomanip>
#include <string>

namespace shak
{
  void cmdArea(const std::vector< Polygon > &polygons, std::istream &in, std::ostream &out);
  void cmdMax(const std::vector< Polygon > &polygons, std::istream &in, std::ostream &out);
  void cmdMin(const std::vector< Polygon > &polygons, std::istream &in, std::ostream &out);
  void cmdCount(const std::vector< Polygon > &polygons, std::istream &in, std::ostream &out);
  void cmdMaxSeq(const std::vector< Polygon > &polygons, std::istream &in, std::ostream &out);
  void cmdRects(const std::vector< Polygon > &polygons, std::ostream &out);
}

#endif
