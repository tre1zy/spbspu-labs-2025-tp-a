#include "commands.hpp"
#include <functional>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <string>
#include <map>
#include <streamGuard.hpp>

namespace
{
  using namespace bocharov;
  using Predicate = std::function<bool(const Polygon &)>;

  struct AreaAccumulator
  {
    Predicate pred;
    double operator()(double sum, const Polygon & poly) const
    {
      return pred(poly) ? sum + getPolygonArea(poly) : sum;
    }
  };

  struct SequenceCounter
  {
    const Polygon & target;
    std::pair<size_t, size_t> operator()(const std::pair<size_t, size_t> & acc, const Polygon & poly) const
    {
      size_t current = (poly == target) ? acc.second + 1 : 0;
      return {std::max(acc.first, current), current};
    }
  };

  void getAreaByPredicate(std::ostream & out, const std::vector<Polygon> & polygons, Predicate pred)
  {
    StreamGuard guard(out);
    AreaAccumulator accumulator{pred};
    double result = std::accumulate(polygons.begin(), polygons.end(), 0.0, accumulator);
    out << std::fixed << std::setprecision(1) << result;
  }

  void getAreaEven(std::ostream & out, const std::vector< Polygon > & polygons)
  {
    getAreaByPredicate(out, polygons, isEven);
  }

  void getAreaOdd(std::ostream & out, const std::vector< Polygon > & polygons)
  {
    getAreaByPredicate(out, polygons, isOdd);
  }

  void getAreaMean(std::ostream & out, const std::vector< Polygon > & polygons)
  {
    if (polygons.empty())
    {
      throw std::runtime_error("ERROR: there are no polygons");
    }
    StreamGuard guard(out);
    std::vector< double > areas;
    areas.reserve(polygons.size());
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), getPolygonArea);
    double result = std::accumulate(areas.begin(), areas.end(), 0.0) / areas.size();
    out << std::fixed << std::setprecision(1) << result;
  }

  void getAreaVertexes(std::ostream & out, const std::vector< Polygon > & polygons, size_t num)
  {
    StreamGuard guard(out);
    std::vector< Polygon > filtered;
    filtered.reserve(polygons.size());
    auto pred = std::bind(hasNVertexes, std::placeholders::_1, num);
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), pred);
    std::vector< double > areas(filtered.size());
    std::transform(filtered.begin(), filtered.end(), std::back_inserter(areas), getPolygonArea);
    double result = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << std::fixed << std::setprecision(1) << result;
  }

  void getMaxArea(std::ostream & out, const std::vector< Polygon > & polygons)
  {
    StreamGuard guard(out);
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), getPolygonArea);
    double result = *std::max_element(areas.begin(), areas.end());
    out << std::fixed << std::setprecision(1) << result;
  }

  void getMaxVertexes(std::ostream & out, const std::vector< Polygon > & polygons)
  {
    StreamGuard guard(out);
    auto result = *std::max_element(polygons.begin(), polygons.end(), compareVertexes);
    out << std::fixed << std::setprecision(1) << result.points.size();
  }

  void getMinArea(std::ostream & out, const std::vector< Polygon > & polygons)
  {
    StreamGuard guard(out);
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), getPolygonArea);
    double result = *std::min_element(areas.begin(), areas.end());
    out << std::fixed << std::setprecision(1) << result;
  }

  void getMinVertexes(std::ostream & out, const std::vector< Polygon > & polygons)
  {
    StreamGuard guard(out);
    auto result = *std::min_element(polygons.begin(), polygons.end(), compareVertexes);
    out << std::fixed << std::setprecision(1) << result.points.size();
  }

  void getCountEven(std::ostream & out, const std::vector< Polygon > & polygons)
  {
    out << std::count_if(polygons.cbegin(), polygons.cend(), isEven);
  }

  void getCountOdd(std::ostream & out, const std::vector< Polygon > & polygons)
  {
    out << std::count_if(polygons.cbegin(), polygons.cend(), isOdd);
  }

  void getCountVertexes(std::ostream & out, const std::vector< Polygon > & polygons, size_t num)
  {
    auto pred = std::bind(hasNVertexes, std::placeholders::_1, num);
    out << std::count_if(polygons.cbegin(), polygons.cend(), pred);
  }

  struct RightAngleCheck
  {
    const Polygon & plg;

    bool operator()(size_t ind) const
    {
      size_t n  = plg.points.size();
      auto prev = plg.points[(ind + n - 1) % n];
      auto curr = plg.points[ind];
      auto next = plg.points[(ind + 1) % n];

      double dx1 = curr.x - prev.x;
      double dy1 = curr.y - prev.y;
      double dx2 = next.x - curr.x;
      double dy2 = next.y - curr.y;

      double scalar = dx1 * dx2 + dy1 * dy2;
      return scalar == 0;
    }
  };

  bool hasRights(const Polygon & plg)
  {
    std::vector< size_t > inds(plg.points.size());
    std::iota(inds.begin(), inds.end(), 0);

    return std::any_of(inds.begin(), inds.end(), RightAngleCheck{ plg });
  }
}

void bocharov::getArea(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["EVEN"] = std::bind(getAreaEven, std::ref(out), std::cref(polygons));
  subcmds["ODD"] = std::bind(getAreaOdd, std::ref(out), std::cref(polygons));
  subcmds["MEAN"] = std::bind(getAreaMean, std::ref(out), std::cref(polygons));

  std::string subcmd;
  in >> subcmd;
  try
  {
    subcmds.at(subcmd)();
  }
  catch (const std::exception &)
  {
    size_t num = std::stoul(subcmd);
    if (num < 3)
    {
      throw std::logic_error("ERROR: the polygon consists of at least 3 vertices");
    }
    getAreaVertexes(out, polygons, num);
  }
}

void bocharov::getMax(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  if (polygons.empty())
  {
    throw std::runtime_error("ERROR: there are no polygons");
  }
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["AREA"] = std::bind(getMaxArea, std::ref(out), std::cref(polygons));
  subcmds["VERTEXES"] = std::bind(getMaxVertexes, std::ref(out), std::cref(polygons));

  std::string subcmd;
  in >> subcmd;
  subcmds.at(subcmd)();
}

void bocharov::getMin(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  if (polygons.empty())
  {
    throw std::runtime_error("ERROR: there are no polygons");
  }
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["AREA"] = std::bind(getMinArea, std::ref(out), std::cref(polygons));
  subcmds["VERTEXES"] = std::bind(getMinVertexes, std::ref(out), std::cref(polygons));

  std::string subcmd;
  in >> subcmd;
  subcmds.at(subcmd)();
}

void bocharov::getCount(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["EVEN"] = std::bind(getCountEven, std::ref(out), std::cref(polygons));
  subcmds["ODD"] = std::bind(getCountOdd, std::ref(out), std::cref(polygons));

  std::string subcmd;
  in >> subcmd;
  try
  {
    subcmds.at(subcmd)();
  }
  catch (const std::exception &)
  {
    size_t num = std::stoul(subcmd);
    if (num < 3)
    {
      throw std::logic_error("ERROR: the polygon consists of at least 3 vertices");
    }
    getCountVertexes(out, polygons, num);
  }
}

void bocharov::getMaxSeqCommand(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  Polygon target;
  in >> target;

  if (!in || in.peek() != '\n')
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  auto init = std::make_pair(0u, 0u);
  SequenceCounter counter{target};
  auto result = std::accumulate(polygons.begin(), polygons.end(), init, counter);
  out << result.first;
}

void bocharov::getRightsCnt(std::ostream & out, const std::vector< Polygon > & polygons)
{
  out << count_if(polygons.cbegin(), polygons.cend(), hasRights);
}
