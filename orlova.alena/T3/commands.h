#ifndef COMMANDS_H
#define COMMANDS_H

#include "geom.h"

namespace orlova
{
  void area(const std::vector< Polygon >&, std::istream&, std::ostream&);
  double areaMean(const std::vector< Polygon >&);
  double areaNum(const std::vector< Polygon >&, size_t);

  void max(const std::vector< Polygon >&, std::istream&, std::ostream&);
  double maxArea(const std::vector< Polygon >&);
  size_t maxVertexes(const std::vector< Polygon >&);

  void min(const std::vector< Polygon >&, std::istream&, std::ostream&);
  double minArea(const std::vector< Polygon >&);
  size_t minVertexes(const std::vector< Polygon >&);

  void count(const std::vector< Polygon >&, std::istream&, std::ostream&);
  size_t countEven(const std::vector< Polygon >&);
  size_t countOdd(const std::vector< Polygon >&);
  size_t countNum(const std::vector< Polygon >&, size_t);

  void perms(const std::vector< Polygon >&, std::istream&, std::ostream&);

  void maxseq(const std::vector< Polygon >&, std::istream&, std::ostream&);
}

#endif
