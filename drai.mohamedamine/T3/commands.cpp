#include "commands.hpp"
#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <sstream>
#include <vector>
#include "polygon.hpp"

namespace amine
{
  struct IntersectsWith
{
  const Polygon& query;
  explicit IntersectsWith(const Polygon& q): query(q) {}
  bool operator()(const Polygon& p) const
  {
    return polygons_intersect(p, query);
  }
};
  struct AreaWithVertexes
{
  int target;
  explicit AreaWithVertexes(int t): target(t) {}
  double operator()(double sum, const Polygon& p) const
  {
    return sum + (static_cast<int>(p.points.size()) == target ? compute_area(p) : 0.0);
  }
};
  struct CountWithVertexes
{
  int target;
  explicit CountWithVertexes(int t): target(t) {}
  bool operator()(const Polygon& p) const
  {
    return static_cast<int>(p.points.size()) == target;
  }
};
  CommandProcessor::CommandProcessor(const std::vector<Polygon>& polygons)
  : polygons_(polygons)
{}
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
  }
  catch (const std::exception&) {
    std::cout << "<INVALID COMMAND>\n";
  }
}

void CommandProcessor::command_area(const std::string& rest) const
{
  struct AreaWithVertexes
  {
    int target;
    explicit AreaWithVertexes(int t): target(t) {}
    double operator()(double sum, const Polygon& p) const
    {
      return sum + (static_cast<int>(p.points.size()) == target ? compute_area(p) : 0.0);
    }
  };

  double result = 0.0;

  if (rest == "EVEN")
  {
    result = std::accumulate(polygons_.begin(), polygons_.end(), 0.0,
      [](double sum, const Polygon& p) {
        return (p.points.size() % 2 == 0) ? sum + compute_area(p) : sum;
      });
  }
  else if (rest == "ODD")
  {
    result = std::accumulate(polygons_.begin(), polygons_.end(), 0.0,
      [](double sum, const Polygon& p) {
        return (p.points.size() % 2 != 0) ? sum + compute_area(p) : sum;
      });
  }
  else if (rest == "MEAN")
  {
    if (polygons_.empty())
    {
      throw std::runtime_error("Invalid command");
    }

    double total = std::accumulate(polygons_.begin(), polygons_.end(), 0.0,
      [](double sum, const Polygon& p) {
        return sum + compute_area(p);
      });

    result = total / polygons_.size();
  }
  else
  {
    bool is_num = !rest.empty() && std::all_of(rest.begin(), rest.end(), ::isdigit);
    if (is_num)
    {
      int target = std::stoi(rest);
      if (target < 3)
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }
      result = std::accumulate(polygons_.begin(), polygons_.end(), 0.0, AreaWithVertexes(target));
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
  }

  std::cout << std::fixed << std::setprecision(1) << result << "\n";
}

void CommandProcessor::command_count(const std::string& rest) const
{
  struct CountWithVertexes
  {
    int target;
    explicit CountWithVertexes(int t): target(t) {}
    bool operator()(const Polygon& p) const
    {
      return static_cast<int>(p.points.size()) == target;
    }
  };

  if (rest == "EVEN")
  {
    size_t result = std::count_if(polygons_.begin(), polygons_.end(),
      [](const Polygon& p) { return p.points.size() % 2 == 0; });
    std::cout << result << "\n";
    return;
  }

  if (rest == "ODD")
  {
    size_t result = std::count_if(polygons_.begin(), polygons_.end(),
      [](const Polygon& p) { return p.points.size() % 2 != 0; });
    std::cout << result << "\n";
    return;
  }

  bool is_num = !rest.empty() && std::all_of(rest.begin(), rest.end(), ::isdigit);
  if (is_num)
  {
    int target = std::stoi(rest);
    if (target < 3)
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    size_t result = std::count_if(polygons_.begin(), polygons_.end(), CountWithVertexes(target));
    std::cout << result << "\n";
    return;
  }

  std::cout << "<INVALID COMMAND>\n";
}

void CommandProcessor::command_max(const std::string& rest) const
{
  if (rest == "AREA") {
  if (polygons_.empty()) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

    double max = compute_area(polygons_[0]);
    if (polygons_.size() > 1) {
      double area = compute_area(polygons_[1]);
      if (area > max) max = area;
    }
    if (polygons_.size() > 2) {
      double area = compute_area(polygons_[2]);
      if (area > max) max = area;
    }
    if (polygons_.size() > 3) {
      double area = compute_area(polygons_[3]);
      if (area > max) max = area;
    }
    if (polygons_.size() > 4) {
      double area = compute_area(polygons_[4]);
      if (area > max) max = area;
    }

    std::cout << std::fixed << std::setprecision(1) << max << "\n";
    return;
  }

  if (rest == "VERTEXES") {
    if (polygons_.empty()) throw std::runtime_error("Invalid command");

    size_t max = polygons_[0].points.size();
    if (polygons_.size() > 1 && polygons_[1].points.size() > max) max = polygons_[1].points.size();
    if (polygons_.size() > 2 && polygons_[2].points.size() > max) max = polygons_[2].points.size();
    if (polygons_.size() > 3 && polygons_[3].points.size() > max) max = polygons_[3].points.size();
    if (polygons_.size() > 4 && polygons_[4].points.size() > max) max = polygons_[4].points.size();

    std::cout << max << "\n";
    return;
  }

  throw std::runtime_error("Invalid command");
}

void CommandProcessor::command_min(const std::string& rest) const
{
  if (rest == "AREA") {
    if (polygons_.empty()) throw std::runtime_error("Invalid command");

    double min = compute_area(polygons_[0]);
    if (polygons_.size() > 1) {
      double area = compute_area(polygons_[1]);
      if (area < min) min = area;
    }
    if (polygons_.size() > 2) {
      double area = compute_area(polygons_[2]);
      if (area < min) min = area;
    }
    if (polygons_.size() > 3) {
      double area = compute_area(polygons_[3]);
      if (area < min) min = area;
    }
    if (polygons_.size() > 4) {
      double area = compute_area(polygons_[4]);
      if (area < min) min = area;
    }

    std::cout << std::fixed << std::setprecision(1) << min << "\n";
    return;
  }

  if (rest == "VERTEXES") {
    if (polygons_.empty()) throw std::runtime_error("Invalid command");

    size_t min = polygons_[0].points.size();
    if (polygons_.size() > 1 && polygons_[1].points.size() < min) min = polygons_[1].points.size();
    if (polygons_.size() > 2 && polygons_[2].points.size() < min) min = polygons_[2].points.size();
    if (polygons_.size() > 3 && polygons_[3].points.size() < min) min = polygons_[3].points.size();
    if (polygons_.size() > 4 && polygons_[4].points.size() < min) min = polygons_[4].points.size();

    std::cout << min << "\n";
    return;
  }

  throw std::runtime_error("Invalid command");
}


void CommandProcessor::command_intersections(const std::string& rest) const
{
  Polygon query;
  if (!parse_polygon(rest, query) || query.points.size() < 3)
  {
    throw std::runtime_error("Invalid polygon");
  }

  struct IntersectsWith
  {
    const Polygon& query;
    explicit IntersectsWith(const Polygon& q): query(q) {}
    bool operator()(const Polygon& p) const
    {
      return polygons_intersect(p, query);
    }
  };

  int result = std::count_if(polygons_.begin(), polygons_.end(), IntersectsWith(query));
  std::cout << result << "\n";
}
void CommandProcessor::command_rmecho(const std::string& rest) const
{
  Polygon query;
  if (!parse_polygon(rest, query) || query.points.size() < 3)
  {
    throw std::runtime_error("Invalid polygon");
  }

  std::vector< Polygon > copy = polygons_;
  EqualToQuery equal_to_query(query);
  UniqueChecker unique_checker(query);

  size_t initial_size = copy.size();
  auto new_end = std::unique(copy.begin(), copy.end(), unique_checker);
  copy.erase(new_end, copy.end());

  std::cout << (initial_size - copy.size()) << "\n";
}
}
