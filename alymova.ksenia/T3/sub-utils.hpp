#ifndef SUB_COMMANDS_HPP
#define SUB_COMMANDS_HPP
#include "shapes.hpp"

namespace alymova
{
  double areaEven(double value, const Polygon& polygon);
  double areaOdd(double value, const Polygon& polygon);
  double areaMean(double value, const Polygon& polygon, size_t size);
  double areaNumber(double value, const Polygon& polygon, size_t vertexes);
  double areaPolygon(const Polygon& polygon);

  double compareMaxArea(double value, const Polygon& polygon);
    size_t compareMaxVertexes(size_t value, const Polygon& polygon);
  double compareMinArea(double value, const Polygon& polygon);
    size_t compareMinVertexes(size_t value, const Polygon& polygon);

  bool isDigit(char c);
  bool isEqualSize(size_t size, const Polygon& polygon);
  size_t getVertexes(std::string str);

  double multPoints(const Point& point1, const Point& point2);
  bool isPolygonEven(const Polygon& polygon);
}
#endif
