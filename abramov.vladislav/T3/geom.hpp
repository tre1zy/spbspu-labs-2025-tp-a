#ifndef GEOM_HPP
#define GEOM_HPP
#include <vector>
#include <iostream>

namespace abramov
{
  struct Point
  {
    int x;
    int y;
  };
  std::istream &operator>>(std::istream &in, Point &p);

  struct Polygon
  {
    std::vector< Point > points;
  };
  std::istream &operator>>(std::istream &in, Polygon &polygon);
};


#endif
