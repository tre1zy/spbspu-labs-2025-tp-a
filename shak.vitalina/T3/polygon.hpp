#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <vector>
#include <iostream>

namespace shak
{
  struct Point
  {
    int x, y;
  };
  std::istream &operator>>(std::istream &in, Point &dest);
  bool operator==(const Point &left, const Point& right);
  bool operator<(const Point &left, const Point &right);
  bool operator<=(const Point &left, const Point &right);
  bool operator>=(const Point &left, const Point &right);
  double getDistance(const Point &point1, const Point &point2);

  struct Polygon
  {
    std::vector< Point > points;
  };
  std::istream &operator>>(std::istream &in, Polygon &dest);
  bool operator==(const std::vector< Point > &left, const std::vector< Point > &right);
  bool isEven(const Polygon &polygon);
  bool isOdd(const Polygon &polygon);
  bool areEqualVertexes(size_t numOfVertexes, const Polygon &polygon);
  bool checkRectangle(const Polygon &polygon);
  double getArea(const Polygon &polygon);
  size_t getVertexes(const Polygon &polygon);

  struct TriangleArea
  {
    Point point1;
    Point point2;
    double operator()(const Point& point3);
  };
}
#endif
