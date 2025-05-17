#ifndef GEOM_HPP
#define GEOM_HPP

#include <list>

namespace aleksandrov
{
  struct Point
  {
    int x;
    int y;
  };

  struct Polygon
  {
    std::list< Point > points;
  };
}

#endif
