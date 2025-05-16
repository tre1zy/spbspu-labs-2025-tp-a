#ifndef SHAPES_H
#define SHAPES_H
#include <vector>

struct Point
{
  int x, y;
};
struct Polygon
{
  std::vector< Point > points;
};

#endif
