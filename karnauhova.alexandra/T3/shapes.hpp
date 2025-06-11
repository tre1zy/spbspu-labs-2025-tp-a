#ifndef SHAPES_HPP
#define SHAPES_HPP
#include <iostream>
#include <vector>

namespace karnauhova
{
  struct Point
  {
    int x, y;
  };
  
  struct Polygon
  {
    std::vector< Point > points;
  };

  struct DelimiterIO
  {
    char exp;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, Point&& point);
  std::istream& operator>>(std::istream& in, Polygon& pol);
}
#endif