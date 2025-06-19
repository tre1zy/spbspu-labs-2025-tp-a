#ifndef AMINE_POLYGON_HPP
#define AMINE_POLYGON_HPP

#include <vector>
#include <string>

namespace amine {

struct Point
{
  int x{};
  int y{};
};

struct Polygon
{
  std::vector< Point > points;

  size_t vertexCount() const;
};

bool parse_polygon(const std::string& str, Polygon& poly);

double compute_area(const Polygon& poly);

bool operator==(const Point& a, const Point& b);
bool operator==(const Polygon& a, const Polygon& b);
bool polygons_intersect(const Polygon& a, const Polygon& b);

}

#endif
