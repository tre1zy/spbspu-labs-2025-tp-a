#ifndef GEOM_HPP
#define GEOM_HPP
#include <vector>

namespace abramov
{
  struct Point
  {
    int x;
    int y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };
};


#endif
