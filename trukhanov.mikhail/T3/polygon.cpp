#include "polygon.hpp"
#include <algorithm>
#include <numeric>
#include <vector>
#include <functional>
#include <iterator>
#include "polygon_utils.hpp"

double trukhanov::getArea(const Polygon& polygon)
{
  const auto& pts = polygon.points;
  std::size_t n = pts.size();
  if (n < 3)
  {
    return 0;
  }

  std::vector< double > areas;
  areas.reserve(n - 2);

  using namespace std::placeholders;
  auto calc = std::bind(VectorProduct{}, pts[0], _1, _2);

  std::transform(pts.begin() + 1, pts.end() - 1,pts.begin() + 2, std::back_inserter(areas), calc);

  return std::accumulate(areas.begin(), areas.end(), 0.0);
}


bool trukhanov::isEven(const Polygon& polygon)
{
  return polygon.points.size() != 0 && polygon.points.size() % 2 == 0;
}

bool trukhanov::isOdd(const Polygon& polygon)
{
  return polygon.points.size() != 0 && polygon.points.size() % 2 != 0;
}

bool trukhanov::compareByVertexes(const Polygon& lhs, const Polygon& rhs)
{
  return lhs.points.size() < rhs.points.size();
}

bool trukhanov::isRight(const Polygon& polygon)
{
  size_t n = polygon.points.size();
  if (n < 3)
  {
    return false;
  }

  std::vector< size_t > indices(n);
  std::iota(indices.begin(), indices.end(), 0);

  return std::any_of(indices.begin(), indices.end(), HasRightAngle(polygon.points));
}

bool trukhanov::operator<(const Point& lhs, const Point& rhs)
{
  return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
}

bool trukhanov::operator==(const Point& lhs, const Point& rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}
