#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include <iostream>
#include <stdexcept>

namespace shiryaeva
{
struct Point
{
  int x, y;
  bool operator==(const Point& other) const
  {
    return x == other.x && y == other.y;
  }
};

struct Polygon
{
  std::vector<Point> points;
  bool operator==(const Polygon& other) const
  {
    return points == other.points;
  }
};

std::istream& operator>>(std::istream& in, Point& point);
std::istream& operator>>(std::istream& in, Polygon& polygon);
double getArea(const Polygon& polygon);
}

#endif
