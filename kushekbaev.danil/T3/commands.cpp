#include "commands.hpp"
#include <functional>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <string>
#include <cmath>
#include <map>
#include "streamguard.hpp"

size_t MIN_NUMBER_OF_VERTICES_IN_POLYGON = 3;

using ull = unsigned long long;

namespace
{
  struct PredicateForVertices
  {
    size_t count;
    bool operator()(const kushekbaev::Polygon& polygon)
    {
      return polygon.points.size() == count;
    }
  };

  struct PointComparator
  {
    int dx = 0;
    int dy = 0;
    bool operator()(const kushekbaev::Point& a, const kushekbaev::Point b)
    {
      return (a.x + dx) == b.x && (a.y + dy) == b.y;
    }
  };

  struct PolygonMatcher
  {
    const kushekbaev::Polygon& target;
    const kushekbaev::Polygon& other;
    bool operator()(const kushekbaev::Point& beforeShift)
    {
      int dx = beforeShift.x - target.points[0].x;
      int dy = beforeShift.y - target.points[0].y;
      return std::equal(target.points.begin(), target.points.end(), other.points.begin(), PointComparator{ dx, dy });
    }
  };

  double getLineLength(kushekbaev::Point first, kushekbaev::Point second)
  {
    double width = (first.x - second.x);
    double length = (first.y - second.y);
    return std::sqrt(width * width + length * length);
  }

  kushekbaev::Polygon makeTriangle(size_t index, const std::vector< kushekbaev::Point >& points)
  {
    return kushekbaev::Polygon{ std::vector< kushekbaev::Point >{ points[0], points[index+1], points[index+2] }};
  }

  void convertPolygonIntoTriangles(const kushekbaev::Polygon& polygon, std::vector< kushekbaev::Polygon >& triangles)
  {
    size_t size = polygon.points.size() - 2;
    std::vector< size_t > indices(size);
    std::iota(indices.begin(), indices.end(), 0);
    auto tmp =  std::bind(makeTriangle, std::placeholders::_1, std::cref(polygon.points));
    std::transform(indices.begin(), indices.end(), std::back_inserter(triangles), tmp);
  }

  double getAreaOfTriangle(const kushekbaev::Polygon& polygon)
  {
    double side1 = getLineLength(polygon.points[0], polygon.points[1]);
    double side2 = getLineLength(polygon.points[1], polygon.points[2]);
    double side3 = getLineLength(polygon.points[0], polygon.points[2]);
    double p = (side1 + side2 + side3) / 2;
    return std::sqrt(p * (p - side1) * (p - side2) * (p - side3));
  }

  double getArea(const kushekbaev::Polygon& polygon)
  {
    std::vector< kushekbaev::Polygon > triangles;
    convertPolygonIntoTriangles(polygon, triangles);
    std::vector< double > areas;
    std::transform(triangles.begin(), triangles.end(), std::back_inserter(areas), getAreaOfTriangle);
    return std::accumulate(areas.begin(), areas.end(), 0);
  }

  bool hasRightAngle(const std::vector< kushekbaev::Point >& polygon)
  {
    if (polygon.size() < MIN_NUMBER_OF_VERTICES_IN_POLYGON)
    {
      return false;
    }
    int amount_of_vertices = polygon.size();
    for (int i = 0; i < amount_of_vertices; ++i)
    {
      const kushekbaev::Point& a = polygon[(i - 1 + amount_of_vertices) % 2];
      const kushekbaev::Point& b = polygon[i];
      const kushekbaev::Point& c = polygon[(i + 1) % 2];
      const double abx = a.x - b.x;
      const double aby = a.y - b.y;
      const double cbx = c.x - b.x;
      const double cby = c.y - b.y;
      const double composition = abx * cbx + aby * cby;
      if (composition == 0)
      {
        return true;
      }
    }
    return false;
  }

  bool isCompabitebleByOverlay(const kushekbaev::Polygon collectedPolygon, const kushekbaev::Polygon inputedPolygon)
  {
    if (collectedPolygon.points.size() != inputedPolygon.points.size())
    {
      return false;
    }
    PolygonMatcher matcher{ collectedPolygon, inputedPolygon };
    return std::any_of(inputedPolygon.points.begin(), inputedPolygon.points.end(), matcher);
  }

  bool isEven(const kushekbaev::Polygon& polygon)
  {
    return polygon.points.size() % 2 == 0;
  }

  bool isOdd(const kushekbaev::Polygon& polygon)
  {
    return !isEven(polygon);
  }

  template< typename Predicate >
  ull totalAreaWithPredicate(const std::vector< kushekbaev::Polygon >& polygons, Predicate predicate)
  {
    std::vector< kushekbaev::Polygon > predicated;
    std::copy_if(polygons.begin(), polygon.end(), std::back_inserter(predicated), predicate);
    std::vector< ull > areas;
    std::transform(predicated.begin(), predicated.end(), std::back_inserter(areas), getArea);
    return std::accumulate(areas.begin(), areas.end(), 0);
  }

  bool compareArea(const kushekbaev::Polygon& polygon1, const kushekbaev::Polygon& polygon2)
  {
    return getArea(polygon1) < getArea(polygon2);
  }

  bool compareNumOfVertices(const kushekbaev::Polygon& polygon1, const kushekbaev::Polygon& polygon2)
  {
    return polygon1.points.size() < polygon2.points.size();
  }

  ull areaEven(const std::vector< kushekbaev::Polygon >& polygons)
  {
    return totalAreaWithPredicate(polygons, isEven);
  }

  ull areaOdd(const std::vector< kushekbaev::Polygon > polygons)
  {
    return totalAreaWithPredicate(polygons, isOdd);
  }

  ull areaMean(const std::vector< kushekbaev::Polygon > polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("There are no polygons in vector!");
    }
    return totalArea(polygons) / polygons.size();
  }

  ull areaNum(const std::vector< kushekbaev::Polygon > polygons, size_t num_of_vertices)
  {
    return totalAreaWithPredicate(polygons, PredicateForVertices{ num_of_vertices });
  }

  void maxArea(const std::vector< kushekbaev::Polygon > polygons, std::ostream& out)
  {
    auto max = (*std::max_element(polygons.begin(), polygons.end(), compareArea));
    StreamGuard s(in);
    out << std::fixed << std::setprecision(1) << getArea(max) << "\n";
  }

  void maxVertices(const std::vector< kushekbaev::Polygon > polygons, std::ostream& out)
  {
    auto max = (*std::max_element(polygons.begin(), polygons.end(), compareNumOfVertices));
    out << max.points.size() << "\n";
  }

  void minArea(const std::vector< kushekbaev::Polygon > polygons, std::ostream& out)
  {
    auto min = (*std::min_element(polygons.begin(), polygons.end(), compareArea));
    StreamGuard s(in);
    out << std::fixed << std::setprecision(1) << getArea(min) << "\n";
  }

  void minVertices(const std::vector< kushekbaev::Polygon > polygons, std::ostream& out)
  {
    auto min = (*std::min_element(polygons.begin(), polygons.end(), compareNumOfVertices));
    out << min.points.size() << "\n";
  }

  void countEven(const std::vector< kushekbaev::Polygon > polygons, std::ostream& out)
  {
    out << std::count_if(polygons.begin(), polygons.end(), isEven);
  }

  void countOdd(const std::vector< kushekbaev::Polygon > polygons, std::ostream& out)
  {
    out << std::count_if(polygons.begin(), polygons.end(), isOdd);
  }

  void countNum(const std::vector< kushekbaev::Polygon > polygons, std::ostream& out, size_t num_of_vertices)
  {
    out << countWithPredicate(polygons, PredicateForVertices{ num_of_vertices });
  }

  ull countWithPredicate(const std::vector< kushekbaev::Polygon >& polygons, PredicateForVertices predicate)
  {
      return std::count_if(polygons.begin(), polygons.end(), predicate);
  }

  ull totalArea(const std::vector<kushekbaev::Polygon>& polygons)
  {
    std::vector< ull > areas;
    areas.reserve(polygons.size());
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), getArea);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }
}

void kushekbaev::area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  double result = 0;
  std::string subcommand;
  in >> subcommand;
  std::map< std::string, std::function< ull() > > subcommands;
  subcommands["EVEN"] = std::bind(areaEven, std::cref(polygons));
  subcommands["ODD"] = std::bind(areaOdd, std::cref(polygons));
  subcommands["MEAN"] = std::bind(areaMean, std::cref(polygons));
  try
  {
    result = subcommands.at(subcommand)();
  }
  catch (...)
  {
    size_t num_of_vertices = std::stoull(subcommand);
    if (num_of_vertices < MIN_NUMBER_OF_VERTICES_IN_POLYGON)
    {
      throw std::logic_error("Too few vertices!");
    }
    result = areaNum(polygons, num_of_vertices);
  }
  StreamGuard s(out);
  out << std::fixed << std::setprecision(1) << result << "\n";
}

void kushekbaev::max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  double result = 0;
  std::string subcommand;
  in >> subcommand;
  std::map< std::string, std::function< void() > > subcommands;
  subcommands["AREA"] = std::bind(maxArea, std::cref(polygons), std::ref(out));
  subcommands["VERTEXES"] = std::bind(maxVertices, std::cref(polygons), std::ref(out));
  try
  {
    subcommands.at(subcommand)();
  }
  catch (...)
  {
    throw std::logic_error("<UNKNOWN COMMAND>");
  }
}

void kushekbaev::min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  double result = 0;
  std::string subcommand;
  in >> subcommand;
  std::map< std::string, std::function< void() > > subcommands;
  subcommands["AREA"] = std::bind(minArea, std::cref(polygons), std::ref(out));
  subcommands["VERTEXES"] = std::bind(minVertices, std::cref(polygons), std::ref(out));
  try
  {
    subcommands.at(subcommand)();
  }
  catch (...)
  {
    throw std::logic_error("<UNKNOWN COMMAND>");
  }
}

void kushekbaev::count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  double result = 0;
  std::string subcommand;
  in >> subcommand;
  std::map< std::string, std::function< void() > > subcommands;
  subcommands["EVEN"] = std::bind(countEven, std::cref(polygons), std::ref(out));
  subcommands["ODD"] = std::bind(countOdd, std::cref(polygons), std::ref(out));
  try
  {
    subcommands.at(subcommand)();
  }
  catch (...)
  {
    size_t num_of_vertices = std::stoull(subcommand);
    if (num_of_vertices < MIN_NUMBER_OF_VERTICES_IN_POLYGON)
    {
      throw std::logic_error("Too few vertices!");
    }
    countNum(polygons, out, num_of_vertices);
  }
}

void kushekbaev::rightshapes(std::ostream& out, const std::vector< Polygon >& polygons)
{
  out << std::count_if(polygons.begin(), polygons.end(), hasRightAngle) << "\n";
}

void kushekbaev::same(std::istream& in, std::ostream out, const std::vector< Polygon >& polygons)
{
  Polygon polygon;
  in >> polygon;
  if (!in || in.peek() != '\n')
  {
    in.clear();
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto tmp = std::bind(isCompabitebleByOverlay, std::placeholders::_1, polygon);
  out << std::count_if(polygons.begin(), polygons.end(), tmp) << "\n";
}
