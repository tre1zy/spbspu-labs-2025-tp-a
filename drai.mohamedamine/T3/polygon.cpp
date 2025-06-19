#include <cmath>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <stdexcept>
#include "polygon.hpp"

namespace amine {

double compute_area(const Polygon& poly)
{
    const auto& pts = poly.points;
    if (pts.size() < 3) return 0.0;

    return 0.5 * std::abs(std::inner_product(
        pts.begin(), pts.end() - 1, 
        pts.begin() + 1, 0.0,
        std::plus<>(),
        [](const Point& a, const Point& b) {
            return a.x * b.y - b.x * a.y;
        }) + 
        (pts.back().x * pts.front().y - pts.front().x * pts.back().y));
}

bool parse_polygon(const std::string& str, Polygon& poly)
{
    std::istringstream iss(str);
    size_t num_points;
    iss >> num_points;

    if (iss.fail() || num_points < 3) {
        return false;
    }

    poly.points.clear();
    poly.points.reserve(num_points);

    for (size_t i = 0; i < num_points; ++i) {
        Point p;
        char ch1, ch2, ch3;
        
        if (!(iss >> ch1 >> p.x >> ch2 >> p.y >> ch3) || 
            ch1 != '(' || ch2 != ';' || ch3 != ')') {
            return false;
        }
        
        poly.points.push_back(p);
    }

    return true;
}

bool operator==(const Point& a, const Point& b)
{
    return a.x == b.x && a.y == b.y;
}

bool operator==(const Polygon& a, const Polygon& b)
{
    if (a.points.size() != b.points.size()) return false;
    return std::equal(a.points.begin(), a.points.end(), b.points.begin());
}

size_t Polygon::vertexCount() const
{
    return points.size();
}

}
