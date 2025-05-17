#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iosfwd>
#include <vector>

#include "geometry.hpp"

namespace kostyukov
{
  void area(std::istream& in, const std::vector< Polygon >& polygon, std::ostream& out);
  void max(std::istream& in, const std::vector< Polygon >& polygon, std::ostream& out);
  void min(std::istream& in, const std::vector< Polygon >& polygon, std::ostream& out);
  void count(std::istream& in, const std::vector< Polygon >& polygon, std::ostream& out);
  void permsCount(std::istream& in, const std::vector< Polygon >& polygon, std::ostream& out);
  void rightShapesCount(std::istream& in, const std::vector< Polygon >& polygon, std::ostream& out);
}
#endif
