#include <cmath>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <stdexcept>
#include "polygon.hpp"

namespace {

int orientation(const amine::Point& a, const amine::Point& b, const amine::Point& c)
{
  int val = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
  return (val > 0) - (val < 0);
}

bool on_segment(const amine::Point& a, const amine::Point& b, const amine::Point& c)
{
  return std::min(a.x, b.x) <= c.x && c.x <= std::max(a.x, b.x) &&
         std::min(a.y, b.y) <= c.y && c.y <= std::max(a.y, b.y);
}

bool segments_intersect(
  const amine::Point& p1,
  const amine::Point& q1,
  const amine::Point& p2,
  const amine::Point& q2
)
{
  int o1 = orientation(p1, q1, p2);
  int o2 = orientation(p1, q1, q2);
  int o3 = orientation(p2, q2, p1);
  int o4 = orientation(p2, q2, q1);

  if (o1 != o2 && o3 != o4) {
    return true;
  }

  return (o1 == 0 && on_segment(p1, q1, p2)) ||
         (o2 == 0 && on_segment(p1, q1, q2)) ||
         (o3 == 0 && on_segment(p2, q2, p1)) ||
         (o4 == 0 && on_segment(p2, q2, q1));
}

}

namespace amine {

double compute_area(const Polygon& poly)
{
  const auto& pts = poly.points;

  if (pts.size() < 3) {
    return 0.0;
  }

  double sum = std::inner_product(
    pts.begin(),
    pts.end() - 1,
    pts.begin() + 1,
    0.0,
    std::plus<>(),
    [](const Point& a, const Point& b) {
      return a.x * b.y - b.x * a.y;
    }
  );

  sum += pts.back().x * pts.front().y - pts.front().x * pts.back().y;

  return 0.5 * std::abs(sum);
}

bool parse_polygon(const std::string& str, Polygon& poly)
{
  std::istringstream iss(str);
  size_t num_points = 0;

  if (!(iss >> num_points) || num_points < 3) {
    return false;
  }

  Polygon temp;
  temp.points.reserve(num_points);

  for (size_t i = 0; i < num_points; ++i) {
    Point p{};
    char ch1 = 0;
    char ch2 = 0;
    char ch3 = 0;

    if (!(iss >> ch1 >> p.x >> ch2 >> p.y >> ch3) ||
        ch1 != '(' || ch2 != ';' || ch3 != ')') {
      return false;
    }

    temp.points.push_back(p);
  }

  poly = std::move(temp);
  return true;
}

bool operator==(const Point& a, const Point& b)
{
  return a.x == b.x && a.y == b.y;
}

bool operator==(const Polygon& a, const Polygon& b)
{
  return a.points.size() == b.points.size() &&
         std::equal(a.points.begin(), a.points.end(), b.points.begin());
}

size_t Polygon::vertexCount() const
{
  return points.size();
}

bool polygons_intersect(const Polygon& a, const Polygon& b)
{
  const auto make_edges = [](const Polygon& poly) {
    std::vector< std::pair< Point, Point > > result;

    std::transform(
      poly.points.begin(),
      poly.points.end() - 1,
      poly.points.begin() + 1,
      std::back_inserter(result),
      [](const Point& p1, const Point& p2) {
        return std::make_pair(p1, p2);
      }
    );

    if (!poly.points.empty()) {
      result.emplace_back(poly.points.back(), poly.points.front());
    }

    return result;
  };

  const auto edgesA = make_edges(a);
  const auto edgesB = make_edges(b);

  return std::any_of(
    edgesA.begin(),
    edgesA.end(),
    [&edgesB](const std::pair< Point, Point >& segA) {
      return std::any_of(
        edgesB.begin(),
        edgesB.end(),
        [&segA](const std::pair< Point, Point >& segB) {
          return segments_intersect(segA.first, segA.second, segB.first, segB.second);
        }
      );
    }
  );
}

}
