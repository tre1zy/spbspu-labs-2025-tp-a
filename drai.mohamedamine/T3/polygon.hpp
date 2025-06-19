#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include <iostream>

namespace amine {

struct Point {
    int x, y;
};

struct Polygon {
    std::vector<Point> points;
    size_t vertexCount() const;
};

double compute_area(const Polygon& poly);
bool parse_polygon(const std::string& str, Polygon& poly);
bool polygons_intersect(const Polygon& a, const Polygon& b);
bool operator==(const Point& a, const Point& b);
bool operator==(const Polygon& a, const Polygon& b);

std::istream& operator>>(std::istream& in, Point& p);
std::istream& operator>>(std::istream& in, Polygon& poly);

}

#endif
