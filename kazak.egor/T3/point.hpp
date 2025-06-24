#ifndef POINT_HPP
#define POINT_HPP
#include <iostream>

struct Point {
  int x, y;

  bool operator==(const Point & other) const;
  bool operator<(const Point & other) const;
};

std::ostream & operator<<(std::ostream & os, const Point & p);
std::istream & operator>>(std::istream & is, Point & p);

#endif
