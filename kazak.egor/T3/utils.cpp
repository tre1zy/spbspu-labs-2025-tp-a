#include "utils.hpp"

namespace utils {

bool polygonInFrame(const Polygon & poly, const Point & minP, const Point & maxP) {
  for (size_t i = 0; i < poly.points.size(); ++i) {
    const Point & p = poly.points[i];
    if (p.x < minP.x || p.x > maxP.x || p.y < minP.y || p.y > maxP.y) {
      return false;
    }
  }
  return true;
}

void boundingBox(const std::vector<Polygon> & polys, Point & minP, Point & maxP) {
  if (polys.empty()) {
    minP = Point{0, 0};
    maxP = Point{0, 0};
    return;
  }
  int minX = std::numeric_limits<int>::max();
  int minY = std::numeric_limits<int>::max();
  int maxX = std::numeric_limits<int>::min();
  int maxY = std::numeric_limits<int>::min();

  for (size_t i = 0; i < polys.size(); ++i) {
    const Polygon & poly = polys[i];
    for (size_t j = 0; j < poly.points.size(); ++j) {
      const Point & p = poly.points[j];
      if (p.x < minX) minX = p.x;
      if (p.y < minY) minY = p.y;
      if (p.x > maxX) maxX = p.x;
      if (p.y > maxY) maxY = p.y;
    }
  }
  minP = Point{minX, minY};
  maxP = Point{maxX, maxY};
}

}
