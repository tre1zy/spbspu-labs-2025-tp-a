#include "commands.hpp"

#include <algorithm>
#include <iomanip>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

#include "geometry.hpp"
#include "../common/scopeGuard.hpp"

namespace
{
  bool isEven(const kostyukov::Polygon& polygon)
  {
    return polygon.points.size() % 2 == 0;
  }

  bool isOdd(const kostyukov::Polygon& polygon)
  {
    return !isEven(polygon);
  }

  bool forAll(const kostyukov::Polygon&)
  {
    return true;
  }

  struct VertexExpected
  {
    size_t count;
    bool operator()(const kostyukov::Polygon& polygon)
    {
      return polygon.points.size() == count;
    }
  };

  template< typename Parameter >
  double sumAreasIf(const std::vector< kostyukov::Polygon >& polygons, Parameter param)
  {
    std::vector< kostyukov::Polygon > suitablePolygon;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(suitablePolygon), param);
    std::vector< double > areas;
    std::transform(suitablePolygon.begin(), suitablePolygon.end(), std::back_inserter(areas), kostyukov::getArea);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  double areaForEven(const std::vector< kostyukov::Polygon >& polygons)
  {
    return sumAreasIf(polygons, isEven);
  }

  double areaForOdd(const std::vector< kostyukov::Polygon >& polygons)
  {
    return sumAreasIf(polygons, isOdd);
  }

  double areaForNum(const std::vector< kostyukov::Polygon >& polygons, size_t countVertexes)
  {
    return sumAreasIf(polygons, VertexExpected{ countVertexes });
  }

  double meanArea(const std::vector< kostyukov::Polygon >& polygons)
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("no polygons for mean area");
    }
    return sumAreasIf(polygons, forAll) / polygons.size();
  }
}

void kostyukov::area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string subcommand;
  in >> subcommand;
  double result = 0.0;
  if (subcommand == "EVEN")
  {
    result = areaForEven(polygons);
  }
  else if (subcommand == "ODD")
  {
    result = areaForOdd(polygons);
  }
  else if (subcommand == "MEAN")
  {
    result = meanArea(polygons);
  }
  else
  {
    size_t countVertexes = 0;
    try
    {
      countVertexes = std::stoull(subcommand);
    }
    catch (const std::invalid_argument& error)
    {
      throw std::invalid_argument("invalid subcommand or number format");
    }
    if (countVertexes < 3)
    {
      throw std::invalid_argument("polygon index must be 3 or greater");
    }
    result = areaForNum(polygons, countVertexes);
  }
  ScopeGuard scopeGrd(out);
  out << std::fixed << std::setprecision(1) << result;
}
