#include "commands.hpp"
#include "polygon.hpp"
#include "format_guard.hpp"
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <stdexcept>
#include <string>
#include <cctype>

namespace shiryaeva
{
double calculateTotalArea(const std::vector< Polygon >& polygons, std::function< bool(size_t) > pred)
{
  return std::accumulate(polygons.begin(), polygons.end(), 0.0,
        [pred](double sum, const Polygon& p) {
            return sum + (pred(p.points.size()) ? getArea(p) : 0.0);
        });
}

void area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string subcmd;
  in >> subcmd;

  FormatGuard guard(out);
  out << std::fixed << std::setprecision(1);

  try
  {
    if (subcmd == "EVEN")
    {
      out << calculateTotalArea(polygons, [](size_t n) { return n % 2 == 0; });
    }
    else if (subcmd == "ODD")
    {
      out << calculateTotalArea(polygons, [](size_t n) { return n % 2 != 0; });
    }
    else if (subcmd == "MEAN")
    {
      if (polygons.empty()) throw std::invalid_argument("");
      double total = calculateTotalArea(polygons, [](size_t) { return true; });
      out << total / polygons.size();
    }
    else
    {
      size_t num = std::stoul(subcmd);
      if (num < 3) throw std::invalid_argument("");
      out << calculateTotalArea(polygons, [num](size_t n) { return n == num; });
    }
  }
  catch (...)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}

void max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  if (polygons.empty())
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  std::string subcmd;
  in >> subcmd;

  if (subcmd == "AREA")
  {
    auto it = std::max_element(polygons.begin(), polygons.end(), 
    [](const Polygon& a, const Polygon& b) { return getArea(a) < getArea(b); });
    FormatGuard guard(out);
    out << std::fixed << std::setprecision(1) << getArea(*it);
  }
  else if (subcmd == "VERTEXES")
  {
    auto it = std::max_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) { return a.points.size() < b.points.size(); });
    out << it->points.size();
  }
  else
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}

void min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  if (polygons.empty())
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  std::string subcmd;
  in >> subcmd;

  if (subcmd == "AREA")
  {
    auto it = std::min_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) { return getArea(a) < getArea(b); });
    FormatGuard guard(out);
    out << std::fixed << std::setprecision(1) << getArea(*it);
  }
  else if (subcmd == "VERTEXES")
  {
    auto it = std::min_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) { return a.points.size() < b.points.size(); });
    out << it->points.size();
  }
  else
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}

void count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string subcmd;
  in >> subcmd;

  try
  {
    if (subcmd == "EVEN")
    {
      out << std::count_if(polygons.begin(), polygons.end(),
                [](const Polygon& p) { return p.points.size() % 2 == 0; });
    }
    else if (subcmd == "ODD")
    {
      out << std::count_if(polygons.begin(), polygons.end(),
                [](const Polygon& p) { return p.points.size() % 2 != 0; });
    }
    else
    {
      size_t num = std::stoul(subcmd);
      if (num < 3) throw std::invalid_argument("");
      out << std::count_if(polygons.begin(), polygons.end(),
                [num](const Polygon& p) { return p.points.size() == num; });
    }
  }
  catch (...)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}

void lessarea(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  Polygon target;
  if (!(in >> target) || target.points.empty())
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  double targetArea = getArea(target);
  out << std::count_if(polygons.begin(), polygons.end(),
  [targetArea](const Polygon& p) { return getArea(p) < targetArea; });
}

void intersections(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  Polygon target;
  if (!(in >> target) || target.points.empty())
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  auto bbIntersects = [](const Polygon& a, const Polygon& b)
  {
    auto [aMinX, aMaxX] = std::minmax_element(a.points.begin(), a.points.end(),
            [](const Point& p1, const Point& p2) { return p1.x < p2.x; });
    auto [aMinY, aMaxY] = std::minmax_element(a.points.begin(), a.points.end(),
            [](const Point& p1, const Point& p2) { return p1.y < p2.y; });
        
    auto [bMinX, bMaxX] = std::minmax_element(b.points.begin(), b.points.end(),
            [](const Point& p1, const Point& p2) { return p1.x < p2.x; });
    auto [bMinY, bMaxY] = std::minmax_element(b.points.begin(), b.points.end(),
            [](const Point& p1, const Point& p2) { return p1.y < p2.y; });

    return !(aMaxX->x < bMinX->x || bMaxX->x < aMinX->x || aMaxY->y < bMinY->y || bMaxY->y < aMinY->y);
  };

  out << std::count_if(polygons.begin(), polygons.end(),
        [&target, bbIntersects](const Polygon& p) { return bbIntersects(target, p); });
}

}
