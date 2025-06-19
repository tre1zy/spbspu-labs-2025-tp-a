#include "polygon.hpp"

#include <cmath>
#include <numeric>
#include <algorithm>

namespace amine
{
  bool Point::operator==(const Point& other) const
  {
    return x == other.x && y == other.y;
  }

  Polygon::Polygon(const std::vector<Point>& points) : points_(points)
  {}

  double Polygon::area() const
  {
    if (points_.size() < 3)
    {
      return 0.0;
    }

    double result = std::inner_product(
      points_.begin(), points_.end() - 1, points_.begin() + 1, 0.0,
      std::plus<>(),
      [](const Point& a, const Point& b)
      {
        return a.x * b.y - b.x * a.y;
      }
    );

    result += points_.back().x * points_.front().y - points_.front().x * points_.back().y;

    return std::abs(result) * 0.5;
  }

  std::size_t Polygon::vertexCount() const
  {
    return points_.size();
  }

  bool Polygon::operator==(const Polygon& other) const
  {
    return points_ == other.points_;
  }
}
