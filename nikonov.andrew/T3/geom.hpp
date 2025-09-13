#ifndef GEOM_H
#define GEOM_H
#include <vector>

namespace nikonov
{
  struct Point
  {
    int x, y;
  };
  struct Polygon
  {
    std::vector< Point > points;
  };
}

#endif
