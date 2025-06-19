#ifndef AMINE_POLYGON_HPP
#define AMINE_POLYGON_HPP

#include <vector>

namespace amine
{
  struct Point
  {
    double x;
    double y;

    bool operator==(const Point& other) const;
  };

  class Polygon
  {
  public:
    explicit Polygon(const std::vector<Point>& points);
    double area() const;
    std::size_t vertexCount() const;
    bool operator==(const Polygon& other) const;

  private:
    std::vector<Point> points_;
  };
}

#endif
