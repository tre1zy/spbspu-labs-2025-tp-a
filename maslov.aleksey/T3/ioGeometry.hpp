#ifndef IO_GEOMETRY_HPP
#define IO_GEOMETRY_HPP

#include <iosfwd>
#include <vector>

namespace maslov
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
  std::ostream & operator<<(std::ostream & out, const Point & dest);
  std::ostream & operator<<(std::ostream & out, const Polygon & dest);
}

#endif
