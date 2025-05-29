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
        double total = std::accumulate(polygons.begin(), polygons.end(), 0.0,
        [](double sum, const Polygon& p) {
          return (p.points.size() % 2 == 0 && p.points.size() >= 3) ? sum + getArea(p) : sum;
                });
        out << total;
      }
      else if (subcmd == "ODD")
      {
        double total = std::accumulate(polygons.begin(), polygons.end(), 0.0,
          [](double sum, const Polygon& p) {
            return (p.points.size() % 2 != 0 && p.points.size() >= 3) ? sum + getArea(p) : sum;
              });
        out << total;
      }
      else if (subcmd == "MEAN")
      {
        if (polygons.empty()) throw std::invalid_argument("<INVALID COMMAND>");
          double total = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [](double sum, const Polygon& p) { return sum + getArea(p); });
        out << total / polygons.size();
      }
      else if (std::all_of(subcmd.begin(), subcmd.end(), ::isdigit))
      {
        size_t num = std::stoul(subcmd);
        if (num < 3) throw std::invalid_argument("<INVALID COMMAND>");
        double total = std::accumulate(polygons.begin(), polygons.end(), 0.0,
          [num](double sum, const Polygon& p) {
            return (p.points.size() == num) ? sum + getArea(p) : sum;
              });
        out << total;
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
      size_t count = 0;
      if (subcmd == "EVEN")
      {
        count = std::count_if(polygons.begin(), polygons.end(),
            [](const Polygon& p) { return p.points.size() % 2 == 0 && p.points.size() >= 3; });
      }
      else if (subcmd == "ODD")
      {
        count = std::count_if(polygons.begin(), polygons.end(),
            [](const Polygon& p) { return p.points.size() % 2 != 0 && p.points.size() >= 3; });
      }
      else (std::all_of(subcmd.begin(), subcmd.end(), ::isdigit))
      {
        size_t num = std::stoul(subcmd);
        if (num < 3)
        {
            throw std::invalid_argument("<INVALID COMMAND>");
        }
        count = std::count_if(polygons.begin(), polygons.end(),
            [num](const Polygon& p) { return p.points.size() == num; });
      }
      out << count;
    }
    catch (...)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
  }

  void lessarea(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons)
  {
    Polygon target;
    if (!(in >> target) || target.points.size() < 3)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    double targetArea = getArea(target);
    size_t count = std::count_if(polygons.begin(), polygons.end(),
        [targetArea](const Polygon& p) {
            return p.points.size() >= 3 && getArea(p) < targetArea;
        });

    out << count;
  }

  void intersections(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons)
  {
    Polygon target;
    if (!(in >> target) || target.points.size() < 3)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    auto bbIntersects = [](const Polygon& a, const Polygon& b)
    {
       if (a.points.empty() || b.points.empty())
       {
        return false;
       }
      auto aMinX = std::min_element(a.points.begin(), a.points.end(),
      [](const Point& p1, const Point& p2) { return p1.x < p2.x; });
      auto aMaxX = std::max_element(a.points.begin(), a.points.end(),
      [](const Point& p1, const Point& p2) { return p1.x < p2.x; });
      auto aMinY = std::min_element(a.points.begin(), a.points.end(),
      [](const Point& p1, const Point& p2) { return p1.y < p2.y; });
      auto aMaxY = std::max_element(a.points.begin(), a.points.end(),
      [](const Point& p1, const Point& p2) { return p1.y < p2.y; });

      auto bMinX = std::min_element(b.points.begin(), b.points.end(),
      [](const Point& p1, const Point& p2) { return p1.x < p2.x; });
      auto bMaxX = std::max_element(b.points.begin(), b.points.end(),
      [](const Point& p1, const Point& p2) { return p1.x < p2.x; });
      auto bMinY = std::min_element(b.points.begin(), b.points.end(),
      [](const Point& p1, const Point& p2) { return p1.y < p2.y; });
      auto bMaxY = std::max_element(b.points.begin(), b.points.end(),
      [](const Point& p1, const Point& p2) { return p1.y < p2.y; });

      return !(aMaxX->x < bMinX->x || bMaxX->x < aMinX->x ||
        aMaxY->y < bMinY->y || bMaxY->y < aMinY->y);
      };

      size_t count = std::count_if(polygons.begin(), polygons.end(),
        [&target, bbIntersects](const Polygon& p) {
            return p.points.size() >= 3 && bbIntersects(target, p);
        });
        
    out << count;
  }

}
