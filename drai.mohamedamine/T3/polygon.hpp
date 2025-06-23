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
struct AreaAccumulator {
    double operator()(double acc, const Polygon& p) const {
        return acc + compute_area(p);
    }
};

struct EvenAreaAccumulator {
    double operator()(double acc, const Polygon& p) const {
        return acc + (p.points.size() % 2 == 0 ? compute_area(p) : 0.0);
    }
};

struct OddAreaAccumulator {
    double operator()(double acc, const Polygon& p) const {
        return acc + (p.points.size() % 2 != 0 ? compute_area(p) : 0.0);
    }
};

struct VertexCountAreaAccumulator {
    int num;
    VertexCountAreaAccumulator(int n) : num(n) {}
    double operator()(double acc, const Polygon& p) const {
        return acc + (static_cast<int>(p.points.size()) == num ? compute_area(p) : 0.0);
    }
};

struct AreaComparator {
    bool operator()(const Polygon& a, const Polygon& b) const {
        return compute_area(a) < compute_area(b);
    }
};

struct VertexCountComparator {
    bool operator()(const Polygon& a, const Polygon& b) const {
        return a.points.size() < b.points.size();
    }
};

struct EvenVertexCountChecker {
    bool operator()(const Polygon& p) const {
        return p.points.size() % 2 == 0;
    }
};

struct OddVertexCountChecker {
    bool operator()(const Polygon& p) const {
        return p.points.size() % 2 != 0;
    }
};

struct SpecificVertexCountChecker {
    int num;
    SpecificVertexCountChecker(int n) : num(n) {}
    bool operator()(const Polygon& p) const {
        return static_cast<int>(p.points.size()) == num;
    }
};

struct PolygonIntersectionChecker {
    const Polygon& query;
    PolygonIntersectionChecker(const Polygon& q) : query(q) {}
    bool operator()(const Polygon& p) const {
        return polygons_intersect(p, query);
    }
};

struct PointGenerator {
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
