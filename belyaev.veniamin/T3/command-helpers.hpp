#ifndef COMMAND_HELPERS_HPP
#define COMMAND_HELPERS_HPP
#include "shapes.hpp"
#include <numeric>
#include <functional>
#include <cmath>
#include <algorithm>

namespace belyaev
{
  double accumulateTerm(double sum, int i, const std::vector<Point>& pnts, int n);
  double calcArea(const Polygon& src);
  bool compareAreas(const Polygon& lhs, const Polygon& rhs);
  bool compareVertices(const Polygon& lhs, const Polygon& rhs);
  bool isPolygonEven(const Polygon& src);
  bool isPolygonOdd(const Polygon& src);
  bool isPolygonOfSize(const Polygon& src, const size_t& userSize);
  bool rmEchoHelper(const Polygon& rmPolygon, const Polygon& lhs, const Polygon& rhs);
  bool isStringNumeric(const std::string& str);
  size_t getVertices(const Polygon& src);
  void compareCoordsMin(const Point& src, int& minX, int& minY);
  void compareCoordsMax(const Point& src, int& minX, int& minY);
  void getMinMaxCoordsInPoly(const Polygon& src, int& minMaxX, int& minMaxY, std::string subCommand);
}

#endif
