#include "commands.hpp"
#include <map>
#include <functional>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <cmath>
#include <streamguard.hpp>
#include <iomanip>

namespace
{
  struct PolyAreaAccumulator
  {
    const tkach::Polygon& poly;
    size_t index;
    double operator()(double area, const tkach::Point& p)
    {
      size_t next = (index + 1) % poly.points.size();
      area += (p.x * poly.points[next].y) - (p.y * poly.points[next].x);
      index++;
      return area;
    }
  };

  double calculatePolygonArea(const tkach::Polygon& polygon)
  {
    if (polygon.points.size() < 3)
    {
      return 0.0;
    }
    PolyAreaAccumulator accum{polygon, 0};
    double sum = std::fabs(std::accumulate(polygon.points.begin(), polygon.points.end(), 0.0, accum));
    return sum / 2.0;
  }

  bool isEven(const tkach::Polygon& polygon)
  {
    return polygon.points.size() % 2 == 0;
  }

  bool isOdd(const tkach::Polygon& polygon)
  {
    return polygon.points.size() % 2 != 0;
  }

  bool checkVertexCount(const tkach::Polygon& polygon, size_t required_count)
  {
    return polygon.points.size() == required_count;
  }

  std::vector< tkach::Polygon > filteredEven(const std::vector< tkach::Polygon >& data)
  {
    if (data.empty())
    {
      throw std::logic_error("Error: zero polygons");
    }
    std::vector< tkach::Polygon > result;
    std::copy_if(data.begin(), data.end(), std::back_inserter(result), isEven);
    return result;
  }

  std::vector< tkach::Polygon > filteredOdd(const std::vector< tkach::Polygon >& data)
  {
    if (data.empty())
    {
      throw std::logic_error("Error: zero polygons");
    }
    std::vector< tkach::Polygon > result;
    std::copy_if(data.begin(), data.end(), std::back_inserter(result), isOdd);
    return result;
  }

  std::vector< tkach::Polygon > filteredMean(const std::vector< tkach::Polygon >& data)
  {
    if (data.empty())
    {
      throw std::logic_error("Error: zero polygons");
    }
    return data;
  }

  std::vector< tkach::Polygon > filteredCount(const std::vector< tkach::Polygon >& data, size_t required_count)
  {
    if (data.empty())
    {
      throw std::logic_error("Error: zero polygons");
    }
    std::vector< tkach::Polygon > result;
    auto predicate = std::bind(checkVertexCount, std::placeholders::_1, required_count);
    std::copy_if(data.begin(), data.end(), std::back_inserter(result), predicate);
    return result;
  }
}

void tkach::printArea(std::istream& in, std::ostream& out, const std::vector< Polygon >& data)
{
  using sub_commands_map = std::map< std::string, std::function< std::vector< Polygon >() > >;
  StreamGuard guard(out);
  sub_commands_map sub_cmds;
  sub_cmds["EVEN"] = std::bind(filteredEven, std::cref(data));
  sub_cmds["ODD"] = std::bind(filteredOdd, std::cref(data));
  sub_cmds["MEAN"] = std::bind(filteredMean, std::cref(data));
  std::string sub_cmd;
  in >> sub_cmd;
  std::vector< tkach::Polygon > filtered; 
  try
  {
    filtered = sub_cmds.at(sub_cmd)();
  }
  catch (...)
  {
    size_t count = std::stoull(sub_cmd);
    if (count < 3)
    {
      throw std::logic_error("Error: no polygon");
    }
    filtered = filteredCount(data, count);
  }
  std::vector< double > areas;
  std::transform(filtered.begin(), filtered.end(), std::back_inserter(areas), calculatePolygonArea);
  out << std::fixed << std::setprecision(1);
  if (sub_cmd == "MEAN")
  {
    out << std::accumulate(areas.begin(), areas.end(), 0.0) / areas.size() << "\n";
  }
  else
  {
    out << std::accumulate(areas.begin(), areas.end(), 0.0) << "\n";
  }
}
