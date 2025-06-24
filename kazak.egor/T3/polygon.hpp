#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <vector>
#include <iostream>
#include "point.hpp"

struct Polygon {
  std::vector<Point> points;

  bool operator==(const Polygon & other) const;
  bool operator<(const Polygon & other) const;

  size_t vertexCount() const;
  double area() const;
};

std::ostream & operator<<(std::ostream & os, const Polygon & poly);
std::istream & operator>>(std::istream & is, Polygon & poly);

bool parsePolygon(const std::string & line, Polygon & poly);

#endif
