#ifndef UTILS_HPP
#define UTILS_HPP
#include <functional>
#include "polygon.hpp"

namespace klimova {
    using namespace std::placeholders;

    double areaEven(double value, const Polygon& polygon, size_t size);
    double areaOdd(double value, const Polygon& polygon, size_t size);
    double areaMean(double value, const Polygon& polygon, size_t size);
    double areaNumber(double value, const Polygon& polygon, size_t vertexes);
    double areaPolygon(const Polygon& polygon);
    double multPoints(const Point& point1, const Point& point2);
    bool isPolygonEven(const Polygon& polygon);
    size_t getVertexes(const std::string& str);

    double getArea(const Polygon& polygon);
    double getVertexesCount(const Polygon& polygon);

    bool isVertexCountEven(const Polygon& polygon);
    bool isVertexCountOdd(const Polygon& polygon);
    bool hasVertexCount(const Polygon& polygon, size_t vertexes);
}
#endif
