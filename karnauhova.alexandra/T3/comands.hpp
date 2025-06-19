#ifndef COMANDS_HPP
#define COMANDS_HPP
#include <iostream>
#include <vector>
#include "shapes.hpp"

namespace karnauhova
{
  void areaComands(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  double evenArea(const std::vector< Polygon >& polygons);
  double oddArea(const std::vector< Polygon >& polygons);
  double meanArea(const std::vector< Polygon >& polygons);
  double countArea(const std::vector< Polygon >& polygons, size_t count);
}
#endif