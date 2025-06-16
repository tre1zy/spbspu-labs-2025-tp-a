#include "utils.hpp"
#include <numeric>
#include <string>
#include <exception>
#include <algorithm>

double klimova::areaEven(double value, const Polygon& polygon) {
    if (isPolygonEven(polygon)) {
        return value + areaPolygon(polygon);
    }
    return value;
}

double klimova::areaOdd(double value, const Polygon& polygon) {
    if (!isPolygonEven(polygon)) {
        return value + areaPolygon(polygon);
    }
    return value;
}

double klimova::areaMean(double value, const Polygon& polygon, size_t size) {
    if (size == 0) {
        throw std::overflow_error("");
    }
    return value + areaPolygon(polygon) / size;
}

double klimova::areaNumber(double value, const Polygon& polygon, size_t vertexes) {
    if (polygon.points.size() == vertexes) {
        return value + areaPolygon(polygon);
    }
    return value;
}

double klimova::areaPolygon(const Polygon& polygon) {
    Polygon polygon_rotate(polygon);
    std::rotate(polygon_rotate.points.begin(), polygon_rotate.points.begin() + 1, polygon_rotate.points.end());
    auto begin = polygon.points.begin();
    auto  end = polygon.points.end();
    double result = std::inner_product(begin, end, polygon_rotate.points.begin(), 0, std::plus< double >{}, multPoints);
    return std::abs(result) / 2.0;
}

double klimova::multPoints(const Point& point1, const Point& point2) {
  return point1.x * point2.y - point1.y * point2.x;
}

bool klimova::isPolygonEven(const Polygon& polygon) {
  return polygon.points.size() % 2 == 0;
}

size_t klimova::getVertexes(const std::string& str) {
    size_t vertexes = std::stoull(str);
    return vertexes;
}

double klimova::getArea(const Polygon& polygon) {
    return areaPolygon(polygon);
}

double klimova::getVertexesCount(const Polygon& polygon) {
    return static_cast<double>(polygon.points.size());
}

bool klimova::isVertexCountEven(const Polygon& polygon) {
    return polygon.points.size() % 2 == 0;
}

bool klimova::isVertexCountOdd(const Polygon& polygon) {
    return !isVertexCountEven(polygon);
}

bool klimova::hasVertexCount(const Polygon& polygon, size_t vertexes) {
    return polygon.points.size() == vertexes;
}

bool klimova::isValidVertexCount(size_t vertexes) {
    return vertexes >= 3;
}

namespace klimova {
    bool PermsTester::operator()(const Polygon& p) const {
        if (p.points.size() != target.points.size()) {
            return false;
        }
        std::vector<Point> p_points = p.points;
        std::vector<Point> target_points = target.points;
        std::sort(p_points.begin(), p_points.end(), PointsComparator());
        std::sort(target_points.begin(), target_points.end(), PointsComparator());
        return std::equal(p_points.begin(), p_points.end(), target_points.begin());
    }

    bool RectangleChecker::is_right_angle(const Point& a, const Point& b, const Point& c) {
        int dx1 = b.x - a.x;
        int dy1 = b.y - a.y;
        int dx2 = b.x - c.x;
        int dy2 = b.y - c.y;
        return (dx1 * dx2 + dy1 * dy2) == 0;
    }

    bool RectangleChecker::operator()(const Polygon& p) const {
        if (p.points.size() != 4) return false;
        bool angle1, angle2, angle3 = false;
        bool angle1 = is_right_angle(p.points[0], p.points[1], p.points[3]);
        bool angle2 = is_right_angle(p.points[1], p.points[2], p.points[0]);
        bool angle3 = is_right_angle(p.points[2], p.points[3], p.points[1]);
        return angle1 && angle2 && angle3;
    }
}
