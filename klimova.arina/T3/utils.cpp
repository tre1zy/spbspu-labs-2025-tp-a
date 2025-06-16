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

size_t klimova::getVertexes(const std::string& str) {
    size_t vertexes = std::stoull(str);
    return vertexes;
}
