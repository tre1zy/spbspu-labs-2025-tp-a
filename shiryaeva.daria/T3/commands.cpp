#include "commands.hpp"
#include <format_guard.hpp>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <string>

namespace shiryaeva
{
  bool hasVertexCount(const Polygon &p, std::size_t count)
  {
    return p.points.size() == count;
  }

  bool intersectsWith(const Polygon &a, const Polygon &b)
  {
    int p1_minX = std::min_element(a.points.cbegin(), a.points.cend(), shiryaeva::comparePointByX)->x;
    int p1_maxX = std::max_element(a.points.cbegin(), a.points.cend(), shiryaeva::comparePointByX)->x;
    int p1_minY = std::min_element(a.points.cbegin(), a.points.cend(), shiryaeva::comparePointByY)->y;
    int p1_maxY = std::max_element(a.points.cbegin(), a.points.cend(), shiryaeva::comparePointByY)->y;

    int p2_minX = std::min_element(b.points.cbegin(), b.points.cend(), shiryaeva::comparePointByX)->x;
    int p2_maxX = std::max_element(b.points.cbegin(), b.points.cend(), shiryaeva::comparePointByX)->x;
    int p2_minY = std::min_element(b.points.cbegin(), b.points.cend(), shiryaeva::comparePointByY)->y;
    int p2_maxY = std::max_element(b.points.cbegin(), b.points.cend(), shiryaeva::comparePointByY)->y;

    bool x_overlap = (p1_minX <= p2_maxX) && (p2_minX <= p1_maxX);
    bool y_overlap = (p1_minY <= p2_maxY) && (p2_minY <= p1_maxY);

    return x_overlap && y_overlap;
  }

  void area(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    FormatGuard guard(out);
    std::string subcmd;
    in >> subcmd;

    std::vector< Polygon > filtered;
    if (subcmd == "EVEN")
    {
      std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), isEven);
    }
    else if (subcmd == "ODD")
    {
      std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), isOdd);
    }
    else if (subcmd == "MEAN")
    {
      if (polygons.empty())
      {
        throw std::logic_error("<INVALID COMMAND>");
      }
      out << std::fixed << std::setprecision(1) << getTotalArea(polygons) / polygons.size();
      return;
    }
    else if (std::all_of(subcmd.begin(), subcmd.end(), ::isdigit))
    {
      std::size_t n = std::stoul(subcmd);
      if (n < MIN_VERTEX_COUNT)
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      std::function< bool(const Polygon&) > pred = std::bind(hasVertexCount, std::placeholders::_1, n);
      std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), pred);
    }
    else
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    out << std::fixed << std::setprecision(1) << getTotalArea(filtered);
  }

  void max(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    FormatGuard guard(out);
    std::string subcmd;
    in >> subcmd;

    if (polygons.empty())
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    out << std::fixed << std::setprecision(1);

    if (subcmd == "AREA")
    {
      auto maxIt = std::max_element(polygons.begin(), polygons.end(), compareByArea);
      out << getPolygonArea(*maxIt);
    }
    else if (subcmd == "VERTEXES")
    {
      auto maxIt = std::max_element(polygons.begin(), polygons.end(), compareByVertexes);
      out << maxIt->points.size();
    }
    else
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
  }

  void min(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    FormatGuard guard(out);
    std::string subcmd;
    in >> subcmd;
    out << std::fixed << std::setprecision(1);

    if (polygons.empty())
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    out << std::fixed << std::setprecision(1);

    if (subcmd == "AREA")
    {
      auto minIt = std::min_element(polygons.begin(), polygons.end(), compareByArea);
      out << getPolygonArea(*minIt);
    }
    else if (subcmd == "VERTEXES")
    {
      auto minIt = std::min_element(polygons.begin(), polygons.end(), compareByVertexes);
      out << minIt->points.size();
    }
    else
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
  }

  void count(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    std::string subcmd;
    in >> subcmd;
    std::size_t cnt = 0;

    if (subcmd == "EVEN")
    {
      cnt = std::count_if(polygons.begin(), polygons.end(), isEven);
    }
    else if (subcmd == "ODD")
    {
      cnt = std::count_if(polygons.begin(), polygons.end(), isOdd);
    }
    else if (std::all_of(subcmd.begin(), subcmd.end(), ::isdigit))
    {
      size_t vertexCount = std::stoul(subcmd);
      if (vertexCount < MIN_VERTEX_COUNT)
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      auto pred = std::bind(hasVertexCount, std::placeholders::_1, vertexCount);
      cnt = std::count_if(polygons.begin(), polygons.end(), pred);
    }
    else
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    out << cnt;
  }

  void lessArea(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    FormatGuard guard(out);
    Polygon polygon;
    in >> polygon;
    if (!in)
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    size_t cnt = std::count_if(
      polygons.begin(), polygons.end(),
      [&polygon](const Polygon &p)
      {
        return getPolygonArea(p) < getPolygonArea(polygon);
      });
    out << cnt;
  }

  void intersections(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    Polygon polygon;
    in >> polygon;
    if ((polygon.points.empty()) || (!in) || (in.peek() != '\n'))
    {
      throw std::logic_error("<INVALID COMMAND>");
    }

    out << std::count_if(polygons.cbegin(), polygons.cend(), std::bind(intersectsWith, polygon, std::placeholders::_1));
  }

  double dot(const Point &a, const Point &b, const Point &c)
  {
    double abx = b.x - a.x;
    double aby = b.y - a.y;
    double bcx = c.x - b.x;
    double bcy = c.y - b.y;
    return abx * bcx + aby * bcy;
  }

  double getDistanceSquared(const Point &a, const Point &b)
  {
    double dx = b.x - a.x;
    double dy = b.y - a.y;
    return dx * dx + dy * dy;
  }

  bool isRightAngle(const Point &prev, const Point &current, const Point &next)
  {
    return std::abs(dot(prev, current, next)) <= EPS;
  }

  bool isRectangle(const Polygon &polygon)
  {
    const auto &points = polygon.points;

    if (points.size() != RECTANGLE_SIDES)
    {
      return false;
    }
    double diag1 = getDistanceSquared(points[0], points[2]);
    double diag2 = getDistanceSquared(points[1], points[3]);

    if (std::abs(diag1 - diag2) > EPS)
    {
      return false;
    }

    return isRightAngle(points[3], points[0], points[1]) &&
      isRightAngle(points[0], points[1], points[2]) &&
      isRightAngle(points[1], points[2], points[3]) &&
      isRightAngle(points[2], points[3], points[0]);
  }
}
