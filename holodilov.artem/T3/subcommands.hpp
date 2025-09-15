#ifndef SUBCOMMANDS_HPP
#define SUBCOMMANDS_HPP

#include <iostream>
#include "polygon.hpp"

namespace holodilov::subcommands
{
  void countEven(std::istream& is, std::ostream& os, const std::vector< Polygon >& vecPolygons);
  void countOdd(std::istream& is, std::ostream& os, const std::vector< Polygon >& vecPolygons);
  void countAmount(std::istream& is, std::ostream& os, const std::vector< Polygon >& vecPolygons, size_t amountVertexes);

  void maxArea(std::istream& is, std::ostream& os, const std::vector< Polygon >& vecPolygons);
  void maxVertexes(std::istream& is, std::ostream& os, const std::vector< Polygon >& vecPolygons);

  void minArea(std::istream& is, std::ostream& os, const std::vector< Polygon >& vecPolygons);
  void minVertexes(std::istream& is, std::ostream& os, const std::vector< Polygon >& vecPolygons);

  void areaEven(std::istream& is, std::ostream& os, const std::vector< Polygon >& vecPolygons);
  void areaOdd(std::istream& is, std::ostream& os, const std::vector< Polygon >& vecPolygons);
  void areaMean(std::istream& is, std::ostream& os, const std::vector< Polygon >& vecPolygons);
  void areaAmount(std::istream& is, std::ostream& os, const std::vector< Polygon >& vecPolygons, size_t amountVertexes);
}

#endif
