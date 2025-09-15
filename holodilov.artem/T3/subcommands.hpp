#ifndef SUBCOMMANDS_HPP
#define SUBCOMMANDS_HPP

#include <iostream>
#include "polygon.hpp"

namespace holodilov::subcommands
{
  void countEven(std::ostream& os, const std::vector< Polygon >& vecPolygons);
  void countOdd(std::ostream& os, const std::vector< Polygon >& vecPolygons);
  void countAmount(std::ostream& os, const std::vector< Polygon >& vecPolygons, size_t amountVertexes);

  void maxArea(std::ostream& os, const std::vector< Polygon >& vecPolygons);
  void maxVertexes(std::ostream& os, const std::vector< Polygon >& vecPolygons);

  void minArea(std::ostream& os, const std::vector< Polygon >& vecPolygons);
  void minVertexes(std::ostream& os, const std::vector< Polygon >& vecPolygons);

  void areaEven(std::ostream& os, const std::vector< Polygon >& vecPolygons);
  void areaOdd(std::ostream& os, const std::vector< Polygon >& vecPolygons);
  void areaMean(std::ostream& os, const std::vector< Polygon >& vecPolygons);
  void areaAmount(std::ostream& os, const std::vector< Polygon >& vecPolygons, size_t amountVertexes);
}

#endif
