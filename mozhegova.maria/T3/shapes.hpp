#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <vector>
#include <istream>

namespace mozhegova
{
  struct Point
  {
    int x, y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };

  std::istream & operator>>(std::istream & in, Point & dest);
  std::istream & operator>>(std::istream & in, Polygon & dest);
}

#endif
