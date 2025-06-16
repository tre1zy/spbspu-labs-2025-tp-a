#include "utils.hpp"
#include <numeric>
#include <string>
#include <exception>
#include <algorithm>

double klimova::areaEven(double value, const Polygon& polygon, size_t size) {
    if (size == 0) {
        return value;
    }
    if (isPolygonEven(polygon)) {
        return value + areaPolygon(polygon);
    }
    return value;
}

double klimova::areaOdd(double value, const Polygon& polygon, size_t size) {
    if (size == 0) {
        return value;
    }
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
