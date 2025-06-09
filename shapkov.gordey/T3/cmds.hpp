#ifndef CMDS_HPP
#define CMDS_HPP
#include <vector>
#include <iostream>
#include "GeometricalTypes.hpp"

namespace shapkov
{
  using VecOfPolygons = std::vector< Polygon >;
  void area(std::istream& in, std::ostream& out, const VecOfPolygons& src);
  void areaEven(std::ostream& out, const VecOfPolygons& src);
  void areaOdd(std::ostream& out, const VecOfPolygons& src);
  void areaMean(std::ostream& out, const VecOfPolygons& src);
  void areaVertexes(std::ostream& out, const VecOfPolygons& src, size_t vertexes);
  void max(std::istream& in, std::ostream& out, const VecOfPolygons& src);
  void min(std::istream& in, std::ostream& out, const VecOfPolygons& src);
  void count(std::istream& in, std::ostream& out, const VecOfPolygons& src);
  void rects(std::ostream& out, const VecOfPolygons& src);
  void same(std::istream& in, std::ostream& out, const VecOfPolygons& src);
}

#endif
