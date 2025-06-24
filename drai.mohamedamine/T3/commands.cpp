#include "commands.hpp"
#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <vector>
#include "polygon.hpp"

namespace amine
{
double areaEven(const std::vector<Polygon>& polys)
{
  double result = 0.0;
  if (polys.size() > 0 && polys[0].points.size() % 2 == 0) result += compute_area(polys[0]);
  if (polys.size() > 1 && polys[1].points.size() % 2 == 0) result += compute_area(polys[1]);
  if (polys.size() > 2 && polys[2].points.size() % 2 == 0) result += compute_area(polys[2]);
  if (polys.size() > 3 && polys[3].points.size() % 2 == 0) result += compute_area(polys[3]);
  if (polys.size() > 4 && polys[4].points.size() % 2 == 0) result += compute_area(polys[4]);
  return result;
}

double areaOdd(const std::vector<Polygon>& polys)
{
  double result = 0.0;
  if (polys.size() > 0 && polys[0].points.size() % 2 != 0) result += compute_area(polys[0]);
  if (polys.size() > 1 && polys[1].points.size() % 2 != 0) result += compute_area(polys[1]);
  if (polys.size() > 2 && polys[2].points.size() % 2 != 0) result += compute_area(polys[2]);
  if (polys.size() > 3 && polys[3].points.size() % 2 != 0) result += compute_area(polys[3]);
  if (polys.size() > 4 && polys[4].points.size() % 2 != 0) result += compute_area(polys[4]);
  return result;
}
double areaMean(const std::vector<Polygon>& polys)
{
  double total = 0.0;
  std::size_t count = polys.size();
  if (count > 0) total += compute_area(polys[0]);
  if (count > 1) total += compute_area(polys[1]);
  if (count > 2) total += compute_area(polys[2]);
  if (count > 3) total += compute_area(polys[3]);
  if (count > 4) total += compute_area(polys[4]);
  return (count > 0) ? total / count : 0.0;
}
double areaNum(const std::vector<Polygon>& polys, int num)
{
  double result = 0.0;
  if (polys.size() > 0 && static_cast<int>(polys[0].points.size()) == num)
    result += compute_area(polys[0]);
  if (polys.size() > 1 && static_cast<int>(polys[1].points.size()) == num)
    result += compute_area(polys[1]);
  if (polys.size() > 2 && static_cast<int>(polys[2].points.size()) == num)
    result += compute_area(polys[2]);
  if (polys.size() > 3 && static_cast<int>(polys[3].points.size()) == num)
    result += compute_area(polys[3]);
  if (polys.size() > 4 && static_cast<int>(polys[4].points.size()) == num)
    result += compute_area(polys[4]);
  return result;
}

  struct AreaComparator
  {
    bool operator()(const Polygon& a, const Polygon& b) const
    {
      return compute_area(a) < compute_area(b);
    }
  };

  struct VertexesComparator
  {
    bool operator()(const Polygon& a, const Polygon& b) const
    {
      return a.points.size() < b.points.size();
    }
  };

  struct EqualToQuery
  {
    const Polygon& query;
    explicit EqualToQuery(const Polygon& q):
      query(q)
    {}

    bool operator()(const Polygon& p) const
    {
      if (p.points.size() != query.points.size())
      {
        return false;
      }

      struct PointEqual
      {
        bool operator()(const Point& a, const Point& b) const
        {
          return a.x == b.x && a.y == b.y;
        }
      };

      return std::equal(p.points.begin(), p.points.end(), query.points.begin(), PointEqual());
    }
  };

  struct UniqueChecker
  {
    EqualToQuery eq;
    explicit UniqueChecker(const Polygon& q):
      eq(q)
    {}

    bool operator()(const Polygon& a, const Polygon& b) const
    {
      return eq(a) && eq(b);
    }
  };

  void process_rmecho(std::vector< Polygon >& polygons, const Polygon& query)
  {
    EqualToQuery equal_to_query(query);
    UniqueChecker unique_checker(query);

    size_t initial_size = polygons.size();
    auto new_end = std::unique(polygons.begin(), polygons.end(), unique_checker);
    polygons.erase(new_end, polygons.end());
    std::cout << (initial_size - polygons.size()) << "\n";
  }

  void CommandProcessor::operator()(const std::string& line) const
{
  if (line.empty()) {
    return;
  }

  std::string::size_type spacePos = line.find(' ');
  std::string cmd;
  std::string rest;

  if (spacePos == std::string::npos) {
    cmd = line;
  } else {
    cmd = line.substr(0, spacePos);
    rest = line.substr(spacePos + 1);
  }

  try {
    bool printDouble = false;
    double dblResult = 0.0;
    int intResult = 0;

    std::map<std::string, void (CommandProcessor::*)(const std::string&) const> handlers = {
      {"AREA", &CommandProcessor::command_area},
      {"COUNT", &CommandProcessor::command_count},
      {"MAX", &CommandProcessor::command_max},
      {"MIN", &CommandProcessor::command_min},
      {"INTERSECTIONS", &CommandProcessor::command_intersections},
      {"RMECHO", &CommandProcessor::command_rmecho}
    };

    auto it = handlers.find(cmd);
    if (it != handlers.end()) {
      (this->*(it->second))(rest);
    } else {
      throw std::runtime_error("Unknown command");
    }

    if (printDouble) {
      std::cout << std::fixed << std::setprecision(1) << dblResult << "\n";
    } else {
      std::cout << intResult << "\n";
    }
  }
  catch (const std::exception&) {
    std::cout << "<INVALID COMMAND>\n";
  };
};
}
