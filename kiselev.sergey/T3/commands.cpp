#include "commands.hpp"
#include <algorithm>
#include <cstddef>
#include <ios>
#include <iterator>
#include <stdexcept>
#include <string>
#include <map>
#include <functional>
#include <iomanip>
#include <vector>
#include <numeric>
#include "polygon.hpp"
#include "scopeGuard.hpp"

namespace
{
  struct EvenPred
  {
    bool operator()(const kiselev::Polygon& poly)
    {
      return poly.points.size() % 2 == 0;
    }
  };

  struct OddPred
  {
    bool operator()(const kiselev::Polygon& poly)
    {
      return poly.points.size() % 2 != 0;
    }
  };

  struct VertexPred
  {
    size_t count;
    bool operator()(const kiselev::Polygon& poly)
    {
      return poly.points.size() == count;
    }
  };

  template< typename Predicate >
  double areaSum(const std::vector< kiselev::Polygon >& polygons, Predicate p)
  {
    std::vector< kiselev::Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), p);
    std::vector< double > areas;
    std::transform(filtered.begin(), filtered.end(), std::back_inserter(areas), kiselev::getArea);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }
  double areaEven(const std::vector< kiselev::Polygon >& polygons)
  {
    return areaSum(polygons, EvenPred());
  }
  double areaOdd(const std::vector< kiselev::Polygon >& polygons)
  {
    return areaSum(polygons, OddPred());
  }
  double areaMean(const std::vector< kiselev::Polygon >& polygons)
  {
    bool all = true;
    return areaSum(polygons, all) / polygons.size();
  }
  double areaNum(const std::vector< kiselev::Polygon >& polygons, size_t n)
  {
    return areaSum(polygons, VertexPred{ n });
  }
}

void kiselev::area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string subcommand;
  in >> subcommand;
  double res = 0;
  std::map< std::string, std::function< double() > > subcommands;
  subcommands["EVEN"] = std::bind(areaEven, std::cref(polygons));
  subcommands["ODD"] = std::bind(areaOdd, std::cref(polygons));
  subcommands["MEAN"] = std::bind(areaMean, std::cref(polygons));
  try
  {
    res = subcommands.at(subcommand)();
  }
  catch (...)
  {
    size_t n = std::stoull(subcommand);
    if (n < 3)
    {
      throw std::logic_error("Few vertices");
    }
    res = areaNum(polygons, n);
  }
  detail::ScopeGuard scope(out);
  out << std::fixed << std::setprecision(1) << res << "\n";
}
