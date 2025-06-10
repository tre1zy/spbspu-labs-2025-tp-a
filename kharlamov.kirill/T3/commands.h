#ifndef COMMANDS_H
#define COMMANDS_H

#include <iosfwd>
#include <vector>
#include "polygon.h"

namespace kharlamov
{
  using PolygonVec = std::vector<Polygon>;
  void maxSeq(std::istream& in, std::ostream& out, const PolygonVec& polygons);
  void same(std::istream& in, std::ostream& out, const PolygonVec& polygons);
}

#endif
