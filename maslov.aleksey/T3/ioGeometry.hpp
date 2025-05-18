#ifndef IO_GEOMETRY_HPP
#define IO_GEOMETRY_HPP

#include <iosfwd>
#include <vector>

namespace maslov
{
  struct Point
  {
    int x, y;
    bool operator==(const Point & rhs) const
    {
      return x == rhs.x && y == rhs.y;
    }
  };

  struct Polygon
  {
    std::vector< Point > points;
    bool operator==(const Polygon & rhs) const
    {
      return points == rhs.points;
    }
  };

  std::istream & operator>>(std::istream & in, Point & dest);
  std::istream & operator>>(std::istream & in, Polygon & dest);
  std::ostream & operator<<(std::ostream & out, const Point & dest);
  std::ostream & operator<<(std::ostream & out, const Polygon & dest);
}

#endif
