#ifndef UTILS_HPP
#define UTILS_HPP
#include <vector>
#include <limits>
#include "polygon.hpp"

namespace utils {

bool polygonInFrame(const Polygon & poly, const Point & minP, const Point & maxP);

void boundingBox(const std::vector<Polygon> & polys, Point & minP, Point & maxP);
}

#endif
