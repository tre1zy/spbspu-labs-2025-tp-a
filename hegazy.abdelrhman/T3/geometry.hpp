#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP
#include <vector>
#include <iosfwd>

namespace geom
{
    struct Point
    {
        int x;
        int y;

        bool operator<(const Point& other) const
        {
            return (x < other.x) || (x == other.x && y < other.y);
        }

        bool operator==(const Point& other) const
        {
            return x == other.x && y == other.y;
        }
    };

    struct Polygon
    {
        std::vector<Point> points;
    };

    bool operator>(const Point& p, const Point& other);
    std::istream& operator>>(std::istream& in, Point& p);
    std::istream& operator>>(std::istream& in, Polygon& poly);
    double getPolygonArea(const Polygon& poly);
    double getDist(const Point& a, const Point& b);
    double getAreaOfTrg(const Polygon& poly);
    Polygon createTrg(size_t i, const std::vector<Point>& points);
    std::vector<Polygon> polyToTrg(const Polygon& poly);


    bool doPolygonsIntersect(const Polygon& poly1, const Polygon& poly2);
    bool doSegmentsIntersect(const Point& p1, const Point& p2, const Point& q1, const Point& q2);
    int orientation(const Point& p, const Point& q, const Point& r);
    bool onSegment(const Point& p, const Point& q, const Point& r);

    struct TrgGenerator
    {
        explicit TrgGenerator(size_t& idx, const std::vector<Point>& pts) :
            points_{ pts }, i_{ idx }
        {
        }
        const std::vector<Point>& points_;
        size_t& i_;
        Polygon operator()();
    };
}

#endif
