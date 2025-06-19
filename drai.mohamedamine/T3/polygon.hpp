#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include <iostream>

namespace amine {

struct Point {
    int x, y;
    bool operator==(const Point& other) const;
};

struct Polygon {
    std::vector<Point> points;
    bool operator==(const Polygon& other) const;
    size_t vertexCount() const;
};

double compute_area(const Polygon& poly);
bool parse_polygon(const std::string& str, Polygon& poly);
bool polygons_intersect(const Polygon& a, const Polygon& b);

std::istream& operator>>(std::istream& in, Point& p);
std::istream& operator>>(std::istream& in, Polygon& poly);

}

#endif
