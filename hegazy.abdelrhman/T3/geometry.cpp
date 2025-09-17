#include "geometry.hpp"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iterator>
#include <limits>
#include <ios>
#include <set>
#include <iostream>
#include <stdexcept>

namespace geom
{
    struct PointReader
    {
        std::istream& in;
        Point operator()() const
        {
            Point p;
            if (!(in >> p)) {
                throw std::runtime_error("Error reading point");
            }
            return p;
        }
    };

    bool operator>(const Point& p, const Point& other)
    {
        return (p.x > other.x) && (p.y > other.y);
    }

    std::istream& operator>>(std::istream& in, Point& p)
    {
        
        char discard;
        if (!(in >> discard >> p.x >> discard >> p.y >> discard)) {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, Polygon& poly)
    {
       
        size_t count;
        if (!(in >> count) || count < 3) {
            in.setstate(std::ios::failbit);
            return in;
        }

        std::vector<Point> pts;
        pts.reserve(count);

        for (size_t i = 0; i < count; ++i) {
            Point p;
            if (!(in >> p)) {
                in.setstate(std::ios::failbit);
                return in;
            }
            pts.push_back(p);
        }

        
        std::set<Point> uniquePoints;
        for (const auto& point : pts) {
            if (!uniquePoints.insert(point).second) {
                in.setstate(std::ios::failbit);
                return in;
            }
        }

        poly.points = std::move(pts);
        return in;
    }

    double getDist(const Point& a, const Point& b)
    {
        return std::sqrt(std::pow((a.x - b.x), 2) + std::pow((a.y - b.y), 2));
    }

    double getAreaOfTrg(const Polygon& poly)
    {
        if (poly.points.size() < 3) return 0.0;
        double side1 = getDist(poly.points[0], poly.points[1]);
        double side2 = getDist(poly.points[0], poly.points[2]);
        double side3 = getDist(poly.points[1], poly.points[2]);
        double p = (side1 + side2 + side3) / 2;
        return std::sqrt(p * (p - side1) * (p - side2) * (p - side3));
    }

    Polygon createTrg(size_t i, const std::vector<Point>& points)
    {
        if (points.size() < i + 3) {
            return Polygon{};
        }
        return Polygon{ std::vector<Point>{ points[0], points[i + 1], points[i + 2] } };
    }

    Polygon TrgGenerator::operator()()
    {
        return createTrg(i_++, points_);
    }

    std::vector<Polygon> polyToTrg(const Polygon& poly)
    {
        if (poly.points.size() < 3) {
            return {};
        }
        size_t size = poly.points.size() - 2;
        size_t ind = 0;
        std::vector<Polygon> triangles(size);
        std::generate(triangles.begin(), triangles.end(), TrgGenerator{ ind, poly.points });
        return triangles;
    }

    double getPolygonArea(const Polygon& poly)
    {
        
        if (poly.points.size() < 3) return 0.0;

        double area = 0.0;
        size_t n = poly.points.size();

        for (size_t i = 0; i < n; ++i) {
            size_t j = (i + 1) % n;
            area += (poly.points[i].x * poly.points[j].y);
            area -= (poly.points[j].x * poly.points[i].y);
        }

        return std::abs(area) / 2.0;
    }

    
    int orientation(const Point& p, const Point& q, const Point& r)
    {
        int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
        if (val == 0) return 0;
        return (val > 0) ? 1 : 2;
    }

    bool onSegment(const Point& p, const Point& q, const Point& r)
    {
        if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
            q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y)) {
            return true;
        }
        return false;
    }

    bool doSegmentsIntersect(const Point& p1, const Point& p2, const Point& q1, const Point& q2)
    {
        int o1 = orientation(p1, p2, q1);
        int o2 = orientation(p1, p2, q2);
        int o3 = orientation(q1, q2, p1);
        int o4 = orientation(q1, q2, p2);

        if (o1 != o2 && o3 != o4) return true;

        if (o1 == 0 && onSegment(p1, q1, p2)) return true;
        if (o2 == 0 && onSegment(p1, q2, p2)) return true;
        if (o3 == 0 && onSegment(q1, p1, q2)) return true;
        if (o4 == 0 && onSegment(q1, p2, q2)) return true;

        return false;
    }

    bool doPolygonsIntersect(const Polygon& poly1, const Polygon& poly2)
    {
        
        for (size_t i = 0; i < poly1.points.size(); ++i) {
            size_t next_i = (i + 1) % poly1.points.size();
            for (size_t j = 0; j < poly2.points.size(); ++j) {
                size_t next_j = (j + 1) % poly2.points.size();
                if (doSegmentsIntersect(poly1.points[i], poly1.points[next_i],
                    poly2.points[j], poly2.points[next_j])) {
                    return true;
                }
            }
        }
        return false;
    }
}