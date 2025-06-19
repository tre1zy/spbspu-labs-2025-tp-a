#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include <iostream>

struct Point {
    int x, y;
    bool operator==(const Point& other) const;
};

struct Polygon {
    std::vector<Point> points;
    bool operator==(const Polygon& other) const;
    size_t vertexCount() const;
};

std::istream& operator>>(std::istream& in, Point& p);
std::istream& operator>>(std::istream& in, Polygon& poly);

#endif
