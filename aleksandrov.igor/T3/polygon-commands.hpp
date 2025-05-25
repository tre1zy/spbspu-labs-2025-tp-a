#ifndef POLYGON_COMMANDS_HPP
#define POLYGON_COMMANDS_HPP

#include <ios>
#include <vector>
#include "geometry.hpp"

namespace aleksandrov
{
  namespace detail
  {
    double multPoints(const Point&, const Point&);
    double calcArea(const Polygon&);
  }
  void getPolygons(std::istream&, std::vector< Polygon >&);
  double areaEven(const std::vector< Polygon >&);
  double areaOdd(const std::vector< Polygon >&);
  double areaMean(const std::vector< Polygon >&);
  double areaN(const std::vector< Polygon >&);
  void area(const std::vector< Polygon >&, std::istream&, std::ostream&);
  void max(const std::vector< Polygon >&, std::ostream&);
  void min(const std::vector< Polygon >&, std::ostream&);
  size_t countEven(const std::vector< Polygon >&);
  size_t countOdd(const std::vector< Polygon >&);
  size_t countN(const std::vector< Polygon >&, size_t);
  void count(const std::vector< Polygon >&, std::istream&, std::ostream&);
}

#endif

