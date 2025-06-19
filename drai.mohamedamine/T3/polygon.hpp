#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include <string>

namespace amine {

struct Point
{
  int x;
  int y;
};

bool operator==(const Point& a, const Point& b);

struct Polygon
{
  std::vector<Point> points;
};

bool parse_polygon(const std::string& str, Polygon& poly);
double compute_area(const Polygon& poly);
bool is_rectangle(const Polygon& poly);
bool polygons_intersect(const Polygon& a, const Polygon& b);
bool point_in_polygon(const Point& pt, const Polygon& poly);
bool seg_intersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2);

}

#endif
