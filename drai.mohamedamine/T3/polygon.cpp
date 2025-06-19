#include <cmath>
#include <iterator>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <functional>
#include <array>
#include <stdexcept>

#include "polygon.hpp"

namespace amine {

double compute_area(const Polygon& poly)
{
    const auto& pts = poly.points;
    if (pts.size() < 3) return 0.0;

    return 0.5 * std::abs(std::inner_product(pts.begin(), pts.end() - 1, pts.begin() + 1, 0.0,
        std::plus<>(),
        [](const Point& a, const Point& b) {
            return a.x * b.y - b.x * a.y;
        }) + (pts.back().x * pts.front().y - pts.front().x * pts.back().y));
}

bool is_rectangle(const Polygon& poly)
{
    if (poly.points.size() != 4) return false;
    const auto& p = poly.points;
    auto dot = [](const Point& a, const Point& b, const Point& c) {
        int abx = b.x - a.x, aby = b.y - a.y;
        int bcx = c.x - b.x, bcy = c.y - b.y;
        return abx * bcx + aby * bcy;
    };
    return dot(p[0], p[1], p[2]) == 0 &&
           dot(p[1], p[2], p[3]) == 0 &&
           dot(p[2], p[3], p[0]) == 0 &&
           dot(p[3], p[0], p[1]) == 0;
}

bool point_in_polygon(const Point& pt, const Polygon& poly)
{
    return std::any_of(poly.points.begin(), poly.points.end(), [&](const Point& a) {
        return a.x == pt.x && a.y == pt.y;
    });
}

bool seg_intersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2)
{
    auto orientation = [](const Point& a, const Point& b, const Point& c) {
        int val = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
        if (val == 0) return 0;
        return (val > 0) ? 1 : 2;
    };

    auto on_segment = [](const Point& a, const Point& b, const Point& c) {
        return std::min(a.x, c.x) <= b.x && b.x <= std::max(a.x, c.x) &&
               std::min(a.y, c.y) <= b.y && b.y <= std::max(a.y, c.y);
    };

    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    return (o1 != o2 && o3 != o4) ||
           (o1 == 0 && on_segment(p1, p2, q1)) ||
           (o2 == 0 && on_segment(p1, q2, q1)) ||
           (o3 == 0 && on_segment(p2, p1, q2)) ||
           (o4 == 0 && on_segment(p2, q1, q2));
}

bool polygons_intersect(const Polygon& a, const Polygon& b)
{
    auto edges = [](const Polygon& p) {
        std::vector<std::pair<Point, Point>> result;
        std::transform(p.points.begin(), std::prev(p.points.end()), std::next(p.points.begin()), std::back_inserter(result),
            [](const Point& p1, const Point& p2) { return std::make_pair(p1, p2); });
        result.emplace_back(p.points.back(), p.points.front());
        return result;
    };

    auto ae = edges(a), be = edges(b);
    return std::any_of(ae.begin(), ae.end(), [&](const auto& segA) {
        return std::any_of(be.begin(), be.end(), [&](const auto& segB) {
            return seg_intersect(segA.first, segA.second, segB.first, segB.second);
        });
    });
}

bool parse_polygon(const std::string& str, Polygon& poly)
{
    std::istringstream iss(str);
    size_t num_points;
    iss >> num_points;

    if (iss.fail() || num_points < 3) return false;

    poly.points.clear();

    for (size_t i = 0; i < num_points; ++i) {
        Point p;
        char ch;
        iss >> ch;
        if (ch != '(') return false;
        iss >> p.x;
        iss >> ch;
        if (ch != ';') return false;
        iss >> p.y;
        iss >> ch;
        if (ch != ')') return false;

        poly.points.push_back(p);
    }

    return poly.points.size() == num_points && iss.eof();
}

bool operator==(const Point& a, const Point& b)
{
    return a.x == b.x && a.y == b.y;
}

bool operator==(const Polygon& a, const Polygon& b)
{
    return a.points == b.points;
}

size_t Polygon::vertexCount() const
{
    return points.size();
}

std::istream& operator>>(std::istream& in, Point& p)
{
    char ch;
    in >> ch >> p.x >> ch >> p.y >> ch;
    if (ch != ')') in.setstate(std::ios::failbit);
    return in;
}

std::istream& operator>>(std::istream& in, Polygon& poly)
{
    size_t num_points;
    in >> num_points;
    if (num_points < 3) {
        in.setstate(std::ios::failbit);
        return in;
    }

    poly.points.resize(num_points);
    for (auto& p : poly.points) {
        in >> p;
        if (in.fail()) break;
    }

    return in;
}

}
