#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>

namespace rychkov
{
  struct Point
  {
    double x, y;
  };
  struct Polygon
  {
    std::vector< Point > vertexes;
  };
}

#endif
