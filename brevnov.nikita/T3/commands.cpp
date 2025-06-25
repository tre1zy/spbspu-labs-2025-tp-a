#include "commands.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <string>
#include <map>
#include <functional>
#include <iomanip>
#include <vector>
#include <numeric>
#include <limits>
#include "polygon.hpp"
#include <streamGuard.hpp>

namespace
{
  bool is_even(const brevnov::Polygon& polygon)
  {
    return polygon.points.size() % 2 == 0;
  }

  bool is_odd(const brevnov::Polygon& polygon)
  {
    return !is_even(polygon);
  }

  bool accept_all(const brevnov::Polygon&)
  {
    return true;
  }

  struct VertexPred
  {
    size_t count;
    bool operator()(const brevnov::Polygon& polygon)
    {
      return polygon.points.size() == count;
    }
  };

  template< typename Predicate >
  double area_sum(const std::vector< brevnov::Polygon >& polygons, Predicate p)
  {
    std::vector< brevnov::Polygon > filter;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filter), p);
    std::vector< double > areas;
    std::transform(filter.begin(), filter.end(), std::back_inserter(areas), brevnov::get_area);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  double area_even(const std::vector< brevnov::Polygon >& polygons)
  {
    return area_sum(polygons, is_even);
  }

  double area_odd(const std::vector< brevnov::Polygon >& polygons)
  {
    return area_sum(polygons, is_odd);
  }

  double area_mean(const std::vector< brevnov::Polygon >& polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("No polygons");
    }
    return area_sum(polygons, accept_all) / polygons.size();
  }

  double area_num(const std::vector< brevnov::Polygon >& polygons, size_t n)
  {
    return area_sum(polygons, VertexPred{ n });
  }

  bool compare_vertex(const brevnov::Polygon& polygon1, const brevnov::Polygon& polygon2)
  {
    return polygon1.points.size() < polygon2.points.size();
  }

  bool compare_area(const brevnov::Polygon& polygon1, const brevnov::Polygon& polygon2)
  {
    return brevnov::get_area(polygon1) < brevnov::get_area(polygon2);
  }

  void max_area(const std::vector< brevnov::Polygon >& polygons, std::ostream& out)
  {
    auto max = (*std::max_element(polygons.begin(), polygons.end(), compare_area));
    brevnov::StreamGuard stream(out);
    out << std::fixed << std::setprecision(1) << brevnov::get_area(max) << "\n";
  }

  void max_vertex(const std::vector< brevnov::Polygon >& polygons, std::ostream& out)
  {
    auto max = (*std::max_element(polygons.begin(), polygons.end(), compare_vertex));
    out << max.points.size() << "\n";
  }

  void min_area(const std::vector< brevnov::Polygon >& polygons, std::ostream& out)
  {
    auto min = (*std::min_element(polygons.begin(), polygons.end(), compare_area));
    brevnov::StreamGuard stream(out);
    out << std::fixed << std::setprecision(1) << brevnov::get_area(min) << "\n";
  }

  void min_vertex(const std::vector< brevnov::Polygon >& polygons, std::ostream& out)
  {
    auto min = (*std::min_element(polygons.begin(), polygons.end(), compare_vertex));
    out << min.points.size() << "\n";
  }

  template< typename Predicate >
  size_t count_if(const std::vector< brevnov::Polygon >& polygons, Predicate p)
  {
    std::vector< brevnov::Polygon > filter;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filter), p);
    return filter.size();
  }

  void count_even(const std::vector< brevnov::Polygon >& polygons, std::ostream& out)
  {
    out << count_if(polygons, is_even) << "\n";
  }

  void count_odd(const std::vector< brevnov::Polygon >& polygons, std::ostream& out)
  {
    out << count_if(polygons, is_odd) << "\n";
  }

  void count_num(const std::vector< brevnov::Polygon >& polygons, std::ostream& out, size_t n)
  {
    out << count_if(polygons, VertexPred{ n }) << "\n";
  }

  int point_f(const brevnov::Point& A, const brevnov::Point& B, const brevnov::Point& C)
  {
    int cross = (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
    if (cross > 0)
    {
      return 1;
    }
    if (cross < 0)
    {
      return -1;
    }
    return 0;
  }

  bool on_segment(const brevnov::Point& A, const brevnov::Point& B, const brevnov::Point& P)
  {
    if (P.x >= std::min(A.x, B.x) && P.x <= std::max(A.x, B.x) && P.y >= std::min(A.y, B.y) && P.y <= std::max(A.y, B.y))
    {
      int cross = (B.x - A.x) * (P.y - A.y) - (B.y - A.y) * (P.x - A.x);
      return cross == 0;
    }
    return false;
  }

  bool segment_insert(const brevnov::Point& A, const brevnov::Point& B, const brevnov::Point& C, const brevnov::Point& D)
  {
    int ccw1 = point_f(A, B, C);
    int ccw2 = point_f(A, B, D);
    int ccw3 = point_f(C, D, A);
    int ccw4 = point_f(C, D, B);
    if (ccw1 * ccw2 < 0 && ccw3 * ccw4 < 0)
    {
      return true;
    }
    if (ccw1 == 0 && ccw2 == 0 && ccw3 == 0 && ccw4 == 0)
    {
      if (std::max(A.x, B.x) < std::min(C.x, D.x) || std::max(C.x, D.x) < std::min(A.x, B.x))
      {
        return false;
      }
      if (std::max(A.y, B.y) < std::min(C.y, D.y) || std::max(C.y, D.y) < std::min(A.y, B.y))
      {
        return false;
      }
      return true;
    }
    bool con1 = ccw1 == 0 && on_segment(A, B, C);
    bool con2 = ccw2 == 0 && on_segment(A, B, D);
    bool con3 = ccw3 == 0 && on_segment(C, D, A);
    bool con4 = ccw4 == 0 && on_segment(C, D, B);
    return con1 || con2 || con3 || con4;
}

  bool check_bounding_box(const brevnov::Polygon& poly1, const brevnov::Polygon& poly2)
  {
    if (poly1.points.empty() || poly2.points.empty())
    {
      return false;
    }
    int min_x1 = poly1.points[0].x, max_x1 = poly1.points[0].x;
    int min_y1 = poly1.points[0].y, max_y1 = poly1.points[0].y;
    for (const auto& point : poly1.points)
    {
      min_x1 = std::min(min_x1, point.x);
      max_x1 = std::max(max_x1, point.x);
      min_y1 = std::min(min_y1, point.y);
      max_y1 = std::max(max_y1, point.y);
    }
    int min_x2 = poly2.points[0].x, max_x2 = poly2.points[0].x;
    int min_y2 = poly2.points[0].y, max_y2 = poly2.points[0].y;
    for (const auto& point : poly2.points)
    {
      min_x2 = std::min(min_x2, point.x);
      max_x2 = std::max(max_x2, point.x);
      min_y2 = std::min(min_y2, point.y);
      max_y2 = std::max(max_y2, point.y);
    }
    return !(max_x1 < min_x2 || max_x2 < min_x1 || max_y1 < min_y2 || max_y2 < min_y1);
  }

  bool point_in_poly(const brevnov::Point& point, const brevnov::Polygon& polygon)
  {
    int n = polygon.points.size();
    if (n < 3)
    {
      return false;
    }
    bool inside = false;
    for (int i = 0, j = n - 1; i < n; j = i++)
    {
      const brevnov::Point& p1 = polygon.points[i];
      const brevnov::Point& p2 = polygon.points[j];
      if ((p1.y > point.y) != (p2.y > point.y))
      {
        if (point.x < (p2.x - p1.x) * (point.y - p1.y) / (p2.y - p1.y) + p1.x)
        {
          inside = !inside;
        }
      }
    }
    return inside;
  }

  bool poly_intersect(const brevnov::Polygon& poly1, const brevnov::Polygon& poly2)
  {
    if (!check_bounding_box(poly1, poly2))
    {
      return false;
    }
    int n1 = poly1.points.size();
    int n2 = poly2.points.size();
    for (int i = 0; i < n1; i++)
    {
      const brevnov::Point& A = poly1.points[i];
      const brevnov::Point& B = poly2.points[(i + 1) % n1];
      for (int j = 0; j < n2; j++)
      {
        const brevnov::Point& C = poly2.points[j];
        const brevnov::Point& D = poly2.points[(j + 1) % 2];
        if (segment_insert(A, B, C, D))
        {
          return true;
        }
      }
    }
    bool con1 = !poly1.points.empty() && point_in_poly(poly1.points[0], poly2);
    bool con2 = !poly2.points.empty() && point_in_poly(poly2.points[0], poly1);
    return con1 || con2;
  }

  struct Check_intersect
  {
    bool operator()(const brevnov::Polygon & a)
    {
      return poly_intersect(a, p);
    }
    const brevnov::Polygon & p;
  };
}

void brevnov::area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string subcommand;
  in >> subcommand;
  double result = 0.0;
  std::map< std::string, std::function< double() > > subcommands;
  subcommands["EVEN"] = std::bind(area_even, std::cref(polygons));
  subcommands["ODD"] = std::bind(area_odd, std::cref(polygons));
  subcommands["MEAN"] = std::bind(area_mean, std::cref(polygons));
  try
  {
    result = subcommands.at(subcommand)();
  }
  catch (...)
  {
    size_t n = std::stoull(subcommand);
    if (n < 3)
    {
      throw std::logic_error("Few vertices");
    }
    result = area_num(polygons, n);
  }
  brevnov::StreamGuard stream(out);
  out << std::fixed << std::setprecision(1) << result << "\n";
}

void brevnov::max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string subcommand;
  in >> subcommand;
  if (polygons.empty())
  {
    throw std::logic_error("No polygons");
  }
  std::map< std::string, std::function< void() > > subcommands;
  subcommands["AREA"] = std::bind(max_area, std::cref(polygons), std::ref(out));
  subcommands["VERTEXES"] = std::bind(max_vertex, std::cref(polygons), std::ref(out));
  try
  {
    subcommands.at(subcommand)();
  }
  catch (...)
  {
    throw std::logic_error("Unknown command");
  }
}

void brevnov::min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string subcommand;
  in >> subcommand;
  if (polygons.empty())
  {
    throw std::logic_error("No polygons");
  }
  std::map< std::string, std::function< void() > > subcommands;
  subcommands["AREA"] = std::bind(min_area, std::cref(polygons), std::ref(out));
  subcommands["VERTEXES"] = std::bind(min_vertex, std::cref(polygons), std::ref(out));
  try
  {
    subcommands.at(subcommand)();
  }
  catch (...)
  {
    throw std::logic_error("Unknown command");
  }
}

void brevnov::count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string subcommand;
  in >> subcommand;
  std::map< std::string, std::function< void() > > subcommands;
  subcommands["EVEN"] = std::bind(count_even, std::cref(polygons), std::ref(out));
  subcommands["ODD"] = std::bind(count_odd, std::cref(polygons), std::ref(out));
  try
  {
    subcommands.at(subcommand)();
  }
  catch (...)
  {
    size_t n = std::stoull(subcommand);
    if (n < 3)
    {
      throw std::logic_error("Few vertices");
    }
    count_num(polygons, out, n);
  }
}

void brevnov::rightshapes(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  if (in.peek() != '\n')
  {
    throw std::logic_error("ERROR: Do not need any parameters");
  }
  out << std::count_if(polygons.cbegin(), polygons.cend(), has_right_angle) << '\n';
}

void brevnov::intersections(std::istream& in, std::ostream& out, const std::vector< Polygon >& data)
{
  Polygon p;
  in >> p;
  if (!in || !(in.peek() == '\n'))
  {
    throw std::logic_error("ERROR: Not correct parameters");
  }
  Check_intersect check{p};
  out << std::count_if(data.begin(),data.end(), check) << '\n';
}
