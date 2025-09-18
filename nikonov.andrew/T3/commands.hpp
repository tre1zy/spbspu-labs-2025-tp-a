#ifndef COMMANDS_H
#define COMMANDS_H
#include <vector>
#include <istream>
#include <ostream>
#include "geom.hpp"
namespace nikonov
{
  void getArea(const std::vector< Polygon >&, std::istream& in, std::ostream& out);
  void getMax(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void getMin(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void getCount(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void getPerms(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void getMaxseq(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
}
#endif
