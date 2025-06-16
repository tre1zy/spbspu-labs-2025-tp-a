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
    size_t getVertexes(const std::string& str);
}
#endif
