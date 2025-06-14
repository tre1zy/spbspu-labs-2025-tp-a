#ifndef GEOMETRIC_SHAPE_HPP
#define GEOMETRIC_SHAPE_HPP
#include <iostream>
#include <vector>
#include <utility>

namespace zakirov
{
  class Polygon
  {
  public:
    std::vector< Point > points_;
  };

  class Point
  {
  public:
    int x_;
    int y_;
  };

  std::istream & operator>>(std::istream & in, Point & point);
  std::istream & operator>>(std::istream & in, Polygon & polygon);
  std::ostream & operator<<(std::ostream & out, Point & point);
}

#endif
