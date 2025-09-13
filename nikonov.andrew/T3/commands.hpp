#ifndef COMMANDS_H
#define COMMANDS_H
#include <vector>
#include <istream>
#include "geom.hpp"
namespace nikonov
{
  void area(const std::vector< Polygon >& polygons, std::istream& in);
  void max(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void min(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void count(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void perms(const std::vector< Polygon >& polygons, std::ostream& out);
  void maxseq(const std::vector< Polygon >& polygons, std::ostream& out);
}

#endif
