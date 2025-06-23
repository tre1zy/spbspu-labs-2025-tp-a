#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <iostream>
#include <string>
#include <vector>

namespace amine
{
    struct Point {
        int x;
        int y;
        Point(int x_val = 0, int y_val = 0): x(x_val), y(y_val) {}

        bool operator==(const Point& other) const {
            return x == other.x && y == other.y;
        }
    };

    struct Polygon {
        std::vector<Point> points;
    };

    struct Line {
        std::string content;
    };

    struct LineToPolygonConverter {
    Polygon operator()(const Line& line) const;
    };

    struct ValidPolygonChecker {
        bool operator()(const Polygon& p) const {
            return p.points.size() >= 3;
        }
    };

    struct InvalidPolygonChecker {
        bool operator()(const Polygon& p) const {
            return p.points.size() < 3;
        }
    };

    std::istream& operator>>(std::istream& is, Line& line);

    Point operator-(const Point& a, const Point& b);
    int dot(const Point& a, const Point& b);
    int cross(const Point& a, const Point& b);

    double compute_area(const Polygon& poly);
    bool is_rectangle(const Polygon& poly);
    long long orient(const Point& p, const Point& q, const Point& r);
    bool on_segment(const Point& p, const Point& q, const Point& r);
    bool seg_intersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2);
    bool point_in_polygon(const Point& pt, const Polygon& poly);
    bool polygons_intersect(const Polygon& a, const Polygon& b);
    bool parse_polygon(const std::string& str, Polygon& poly);

    void process_commands(std::vector<Polygon>& polygons);
}

#endif
