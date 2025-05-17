#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP
#include <iosfwd>
#include <vector>

namespace kostyukov
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

  std::istream& operator>>(std::istream&, Point&);
  std::istream& operator>>(std::istream&, Polygon&);
}
#endif
