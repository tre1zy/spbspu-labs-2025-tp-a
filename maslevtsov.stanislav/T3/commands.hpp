#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "shapes.hpp"

namespace maslevtsov {
  void get_area(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void get_max(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void get_min(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void count_vertexes(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void echo(std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void remove_echo(std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
}

#endif
