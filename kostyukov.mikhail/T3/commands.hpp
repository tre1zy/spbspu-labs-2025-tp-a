#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iosfwd>
#include <vector>

#include "geometry.hpp"

namespace kostyukov
{
  void area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygon);
  void min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygon);
  void count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygon);
  void permsCount(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygon);
  void rightShapesCount(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygon);
}
#endif
