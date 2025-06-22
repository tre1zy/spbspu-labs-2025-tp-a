#include "polygon.hpp"
#include <algorithm>
#include <cmath>
#include <functional>
#include <numeric>
#include <sstream>

namespace amine {

static const std::size_t RECT_VERTICES = 4;

Point operator-(const Point& a, const Point& b)
{
  return Point(a.x - b.x, a.y - b.y);
}

int dot(const Point& a, const Point& b)
{
  return a.x * b.x + a.y * b.y;
}

int cross(const Point& a, const Point& b)
{
  return a.x * b.y - a.y * b.x;
}

struct ShoelaceAccumulator {
  long long operator()(long long acc, const std::pair<Point, Point>& points) const {
    const Point& a = points.first;
    const Point& b = points.second;
    return acc + static_cast<long long>(a.x) * b.y - static_cast<long long>(a.y) * b.x;
  }
};

double compute_area(const Polygon& poly)
{
  const auto& pts = poly.points;
  if (pts.size() < 3) return 0.0;

  std::vector<std::pair<Point, Point>> point_pairs;
  std::transform(pts.begin(), pts.end() - 1, pts.begin() + 1,
                std::back_inserter(point_pairs),
                [](const Point& a, const Point& b) { return std::make_pair(a, b); });

  long long area2 = std::accumulate(point_pairs.begin(), point_pairs.end(), 0LL, ShoelaceAccumulator{});

  const Point& last = pts.back();
  const Point& first = pts.front();
  area2 += static_cast<long long>(last.x) * first.y - static_cast<long long>(last.y) * first.x;

  return std::abs(area2) / 2.0;
}

struct OrthogonalChecker {
  const Point& prev;
  bool operator()(const Point& current) const {
    Point vec = current - prev;
    return dot(vec, Point{vec.y, -vec.x}) == 0;
  }
};

bool is_rectangle(const Polygon& poly)
{
  if (poly.points.size() != RECT_VERTICES) return false;

  std::vector<Point> shifted(poly.points);
  std::rotate(shifted.begin(), shifted.begin() + 1, shifted.end());

  std::vector<int> dots;
  std::transform(poly.points.begin(), poly.points.end(), shifted.begin(),
                std::back_inserter(dots), dot);

  return std::all_of(dots.begin(), dots.end(), std::bind(std::equal_to<int>(), std::placeholders::_1, 0));
}

long long orient(const Point& p, const Point& q, const Point& r)
{
  return static_cast<long long>(q.x - p.x) * (r.y - p.y) -
         static_cast<long long>(q.y - p.y) * (r.x - p.x);
}

bool on_segment(const Point& p, const Point& q, const Point& r)
{
  return q.x >= std::min(p.x, r.x) && q.x <= std::max(p.x, r.x) &&
         q.y >= std::min(p.y, r.y) && q.y <= std::max(p.y, r.y);
}

struct SegmentIntersectionChecker {
  const Point& p1;
  const Point& q1;
  bool operator()(const std::pair<Point, Point>& seg2) const {
    const Point& p2 = seg2.first;
    const Point& q2 = seg2.second;
    long long o1 = orient(p1, q1, p2);
    long long o2 = orient(p1, q1, q2);
    long long o3 = orient(p2, q2, p1);
    long long o4 = orient(p2, q2, q1);

    if (o1 * o2 < 0 && o3 * o4 < 0) return true;
    if (o1 == 0 && on_segment(p1, p2, q1)) return true;
    if (o2 == 0 && on_segment(p1, q2, q1)) return true;
    if (o3 == 0 && on_segment(p2, p1, q2)) return true;
    if (o4 == 0 && on_segment(p2, q1, q2)) return true;
    return false;
  }
};

bool seg_intersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2)
{
  return SegmentIntersectionChecker{p1, q1}(std::make_pair(p2, q2));
}

struct PointInPolygonChecker {
  const Point& pt;
  mutable bool inside;
  mutable Point prev;

  void operator()(const Point& current) const {
    bool cond = (current.y > pt.y) != (prev.y > pt.y);
    bool intersect = cond &&
      (pt.x < static_cast<long double>(prev.x - current.x) * (pt.y - current.y) / (prev.y - current.y) + current.x);
    if (intersect) inside = !inside;
    prev = current;
  }
};

bool point_in_polygon(const Point& pt, const Polygon& poly)
{
  if (poly.points.size() < 3) return false;

  PointInPolygonChecker checker{pt, false, poly.points.back()};
  std::for_each(poly.points.begin(), poly.points.end(), checker);
  return checker.inside;
}

struct PolygonIntersectionChecker {
  const Polygon& a;
  const Polygon& b;
  bool operator()(const Point& p1) const {
    int i = &p1 - &a.points[0];
    return std::any_of(b.points.begin(), b.points.end(), [this, i](const Point& p2) {
      int j = &p2 - &b.points[0];
      return seg_intersect(a.points[i], a.points[(i + 1) % a.points.size()],
                          b.points[j], b.points[(j + 1) % b.points.size()]);
    });
  }
};

bool polygons_intersect(const Polygon& a, const Polygon& b)
{
  if (a.points.empty() || b.points.empty()) return false;

  bool segment_intersection = std::any_of(a.points.begin(), a.points.end(),
                                         PolygonIntersectionChecker{a, b});

  if (segment_intersection) return true;
  return point_in_polygon(a.points[0], b) || point_in_polygon(b.points[0], a);
}

struct PointParser {
  std::istringstream& iss;
  Point operator()() const {
    Point pt;
    char c;
    if (!(iss >> c) || c != '(') return Point{};
    if (!(iss >> pt.x)) return Point{};
    if (!(iss >> c) || c != ';') return Point{};
    if (!(iss >> pt.y)) return Point{};
    if (!(iss >> c) || c != ')') return Point{};
    return pt;
  }
};

bool parse_polygon(const std::string& str, Polygon& poly)
{
  std::istringstream iss(str);
  int n;
  if (!(iss >> n) || n <= 0) return false;

  poly.points.clear();
  poly.points.reserve(n);

  std::generate_n(std::back_inserter(poly.points), n, PointParser{iss});

  iss >> std::ws;
  return !iss.fail() && iss.eof() && poly.points.size() == static_cast<size_t>(n);
}

}
