#ifndef COMMANDS_H
#define COMMANDS_H
#include <vector>
#include <istream>
#include <ostream>
#include "geom.hpp"
namespace nikonov
{
  void calcArea(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void calcMax(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void calcMin(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void calcCount(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void calcPerms(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void calcMaxseq(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
}
#endif
