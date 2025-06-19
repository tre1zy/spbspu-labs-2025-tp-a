#include <cmath>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <stdexcept>
#include "polygon.hpp"

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

  const auto parse_point = [&iss](Point& p) -> bool {
    char ch1 = 0;
    char ch2 = 0;
    char ch3 = 0;

    return static_cast<bool>(
      iss >> ch1 >> p.x >> ch2 >> p.y >> ch3 &&
      ch1 == '(' && ch2 == ';' && ch3 == ')'
    );
  };

  const bool success = std::all_of(
    std::make_move_iterator(std::vector<size_t>(num_points).begin()),
    std::make_move_iterator(std::vector<size_t>(num_points).end()),
    [&temp, &parse_point](size_t) {
      Point p{};
      if (!parse_point(p)) {
        return false;
      }
      temp.points.push_back(p);
      return true;
    }
  );

  if (!success) {
    return false;
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
  return a.points.size() == b.points.size()
    && std::equal(a.points.begin(), a.points.end(), b.points.begin());
}

size_t Polygon::vertexCount() const
{
  return points.size();
}

}
