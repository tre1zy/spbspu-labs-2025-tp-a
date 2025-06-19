#include <iostream>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <iterator>

#include "polygon.hpp"

namespace amine {

static const std::size_t RECT_VERTICES = 4;

Point operator-(const Point& a, const Point& b)
{
  return Point{a.x - b.x, a.y - b.y};
}

int dot(const Point& a, const Point& b)
{
  return a.x * b.x + a.y * b.y;
}

int cross(const Point& a, const Point& b)
{
  return a.x * b.y - a.y * b.x;
}

double compute_area(const Polygon& poly)
{
  const auto& pts = poly.points;
  if (pts.size() < 3) {
    return 0.0;
  }

  long long area2 = std::inner_product(
    pts.begin(), pts.end() - 1, pts.begin() + 1, 0LL,
    std::plus<>(),
    [](const Point& a, const Point& b) {
      return static_cast<long long>(a.x) * b.y - static_cast<long long>(a.y) * b.x;
    });

  area2 += static_cast<long long>(pts.back().x) * pts.front().y
         - static_cast<long long>(pts.back().y) * pts.front().x;

  return std::abs(area2) / 2.0;
}

bool is_rectangle(const Polygon& poly)
{
  if (poly.points.size() != RECT_VERTICES) {
    return false;
  }

  const auto& p = poly.points;
  std::array<Point, 4> v = {
    p[1] - p[0],
    p[2] - p[1],
    p[3] - p[2],
    p[0] - p[3]
  };

  bool all_right_angles = (dot(v[0], v[1]) == 0)
                       && (dot(v[1], v[2]) == 0)
                       && (dot(v[2], v[3]) == 0)
                       && (dot(v[3], v[0]) == 0);
  if (!all_right_angles) {
    return false;
  }

  auto square_length = [](const Point& v) {
    return static_cast<long long>(v.x) * v.x + static_cast<long long>(v.y) * v.y;
  };

  return square_length(v[0]) == square_length(v[2])
      && square_length(v[1]) == square_length(v[3]);
}

long long orient(const Point& p, const Point& q, const Point& r)
{
  return static_cast<long long>(q.x - p.x) * (r.y - p.y)
       - static_cast<long long>(q.y - p.y) * (r.x - p.x);
}

bool on_segment(const Point& p, const Point& q, const Point& r)
{
  return q.x >= std::min(p.x, r.x) && q.x <= std::max(p.x, r.x)
      && q.y >= std::min(p.y, r.y) && q.y <= std::max(p.y, r.y);
}

bool seg_intersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2)
{
  long long o1 = orient(p1, q1, p2);
  long long o2 = orient(p1, q1, q2);
  long long o3 = orient(p2, q2, p1);
  long long o4 = orient(p2, q2, q1);

  if (o1 * o2 < 0 && o3 * o4 < 0) {
    return true;
  }

  return (o1 == 0 && on_segment(p1, p2, q1))
      || (o2 == 0 && on_segment(p1, q2, q1))
      || (o3 == 0 && on_segment(p2, p1, q2))
      || (o4 == 0 && on_segment(p2, q1, q2));
}

bool point_in_polygon(const Point& pt, const Polygon& poly)
{
  const auto& p = poly.points;
  if (p.size() < 3) {
    return false;
  }

  std::size_t n = p.size();
  bool inside = false;
  std::for_each(p.begin(), p.end(), [&, i = std::size_t(0), j = n - 1](const Point&) mutable {
    bool cond = (p[i].y > pt.y) != (p[j].y > pt.y);
    bool intersect = cond && (pt.x < static_cast<long double>(p[j].x - p[i].x)
                                         * (pt.y - p[i].y) / (p[j].y - p[i].y) + p[i].x);
    if (intersect) {
      inside = !inside;
    }
    j = i++;
  });

  return inside;
}

bool polygons_intersect(const Polygon& a, const Polygon& b)
{
  if (a.points.empty() || b.points.empty()) {
    return false;
  }

  bool segments_intersect = std::any_of(a.points.begin(), a.points.end(), [&a, &b](const Point& p1) {
    auto next_a = (&p1 == &a.points.back()) ? a.points.front() : *(std::next(&p1));
    return std::any_of(b.points.begin(), b.points.end(), [&p1, &next_a, &b](const Point& p2) {
      auto next_b = (&p2 == &b.points.back()) ? b.points.front() : *(std::next(&p2));
      return seg_intersect(p1, next_a, p2, next_b);
    });
  });

  if (segments_intersect) {
    return true;
  }

  return point_in_polygon(a.points[0], b) || point_in_polygon(b.points[0], a);
}

bool parse_polygon(const std::string& str, Polygon& poly)
{
  std::istringstream iss(str);
  int n;
  if (!(iss >> n) || n <= 0) {
    return false;
  }

  poly.points.clear();
  poly.points.reserve(n);

  for (int i = 0; i < n; ++i) {
    char c;
    Point pt;
    if (!(iss >> c) || c != '(' || !(iss >> pt.x)
        || !(iss >> c) || c != ';' || !(iss >> pt.y)
        || !(iss >> c) || c != ')') {
      return false;
    }
    poly.points.push_back(pt);
  }

  iss >> std::ws;
  return iss.eof() && poly.points.size() == static_cast<std::size_t>(n);
}

}
