#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <iostream>
#include <string>
#include <vector>

namespace amine
{

  struct Point
  {
    int x;
    int y;
    Point(int x_val = 0, int y_val = 0):
      x(x_val),
      y(y_val)
    {}
  };

  struct Polygon
  {
    std::vector< Point > points;
  };

  struct Line
  {
    std::string content;
  };
  struct PointPairArea {
    long long operator()(const Point& a, const Point& b) const {
        return static_cast<long long>(a.x) * b.y - static_cast<long long>(a.y) * b.x;
    }
};

struct PointInPolygonAccumulator {
    const Point& pt;
    bool& inside;
    std::pair<Point, bool> operator()(std::pair<Point, bool> acc, const Point& current) const {
        const Point& prev = acc.first;
        bool cond = (current.y > pt.y) != (prev.y > pt.y);
        bool intersect = cond && (pt.x < static_cast<long double>(prev.x - current.x) * (pt.y - current.y) / (prev.y - current.y) + current.x);
        if (intersect) inside = !inside;
        return std::make_pair(current, inside);
    }
};

struct PolygonIntersectionChecker {
    const Polygon& a;
    const Polygon& b;
    int na, nb;
    bool operator()(const Point& p1) const {
        int i = &p1 - &a.points[0];
        return std::any_of(b.points.begin(), b.points.end(), SegmentIntersectionChecker{a, b, i, na, nb});
    }
};

struct SegmentIntersectionChecker {
    const Polygon& a;
    const Polygon& b;
    int i, na, nb;
    bool operator()(const Point& p2) const {
        int j = &p2 - &b.points[0];
        return seg_intersect(a.points[i], a.points[(i + 1) % na], b.points[j], b.points[(j + 1) % nb]);
    }
};

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

struct PolygonValidator {
    bool operator()(const Polygon& p) const {
        return !p.points.empty();
    }
};

struct LineToPolygonConverter {
    Polygon operator()(const Line& line) const {
        Polygon poly;
        if (!line.content.empty() && parse_polygon(line.content, poly) && poly.points.size() >= 3) {
            return poly;
        }
        return Polygon{};
    }
};

  std::istream& operator>>(std::istream& is, Line& line);

  Point operator-(const Point& a, const Point& b);
  int dot(const Point& a, const Point& b);
  int cross(const Point& a, const Point& b);

  double compute_area(const Polygon& poly);
  bool is_rectangle(const Polygon& poly);
  long long orient(const Point& p, const Point& q, const Point& r);
  bool on_segment(const Point& p, const Point& q, const Point& r);
  bool seg_intersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2);
  bool point_in_polygon(const Point& pt, const Polygon& poly);
  bool polygons_intersect(const Polygon& a, const Polygon& b);
  bool parse_polygon(const std::string& str, Polygon& poly);

  void process_commands(std::vector< Polygon >& polygons);

}

#endif
