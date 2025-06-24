#include <sstream>
#include "point.hpp"

bool Point::operator==(const Point & other) const {
  return x == other.x && y == other.y;
}

bool Point::operator<(const Point & other) const {
  if (x != other.x)
    return x < other.x;
  return y < other.y;
}

std::ostream & operator<<(std::ostream & os, const Point & p) {
  return os << "(" << p.x << ";" << p.y << ")";
}

std::istream & operator>>(std::istream & is, Point & p) {
  char c1 = 0, c2 = 0, c3 = 0;
  int x = 0, y = 0;

  is >> std::ws;
  if (!(is >> c1) || c1 != '(') {
    is.setstate(std::ios::failbit);
    return is;
  }
  if (!(is >> x)) {
    is.setstate(std::ios::failbit);
    return is;
  }
  if (!(is >> c2) || c2 != ';') {
    is.setstate(std::ios::failbit);
    return is;
  }
  if (!(is >> y)) {
    is.setstate(std::ios::failbit);
    return is;
  }
  if (!(is >> c3) || c3 != ')') {
    is.setstate(std::ios::failbit);
    return is;
  }
  p.x = x;
  p.y = y;
  return is;
}

