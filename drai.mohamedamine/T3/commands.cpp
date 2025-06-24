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
void CommandProcessor::command_area(const std::string& rest) const {
  double result = 0.0;

  if (rest == "EVEN") {
    result = areaEven(polygons_);
  } else if (rest == "ODD") {
    result = areaOdd(polygons_);
  } else if (rest == "MEAN") {
    result = areaMean(polygons_);
  } else {
    std::istringstream iss(rest);
    int num;
    if (!(iss >> num) || num < 3) {
      throw std::runtime_error("Invalid argument");
    }
    result = areaNum(polygons_, num);
  }

  std::cout << std::fixed << std::setprecision(1) << result << "\n";
}

void CommandProcessor::command_count(const std::string& rest) const {
  std::istringstream iss(rest);
  int num;
  if (!(iss >> num) || num < 3) {
    throw std::runtime_error("Invalid argument");
  }

  int count = 0;
  if (polygons_.size() > 0 && static_cast<int>(polygons_[0].points.size()) == num) ++count;
  if (polygons_.size() > 1 && static_cast<int>(polygons_[1].points.size()) == num) ++count;
  if (polygons_.size() > 2 && static_cast<int>(polygons_[2].points.size()) == num) ++count;
  if (polygons_.size() > 3 && static_cast<int>(polygons_[3].points.size()) == num) ++count;
  if (polygons_.size() > 4 && static_cast<int>(polygons_[4].points.size()) == num) ++count;

  std::cout << count << "\n";
}

void CommandProcessor::command_max(const std::string& rest) const {
  if (!rest.empty()) throw std::runtime_error("Unexpected argument");

  double maxArea = 0.0;
  if (polygons_.size() > 0) maxArea = compute_area(polygons_[0]);
  if (polygons_.size() > 1) maxArea = std::max(maxArea, compute_area(polygons_[1]));
  if (polygons_.size() > 2) maxArea = std::max(maxArea, compute_area(polygons_[2]));
  if (polygons_.size() > 3) maxArea = std::max(maxArea, compute_area(polygons_[3]));
  if (polygons_.size() > 4) maxArea = std::max(maxArea, compute_area(polygons_[4]));

  std::cout << std::fixed << std::setprecision(1) << maxArea << "\n";
}

void CommandProcessor::command_min(const std::string& rest) const {
  if (!rest.empty()) throw std::runtime_error("Unexpected argument");

  if (polygons_.empty()) {
    std::cout << "0.0\n";
    return;
  }

  double minArea = compute_area(polygons_[0]);
  if (polygons_.size() > 1) minArea = std::min(minArea, compute_area(polygons_[1]));
  if (polygons_.size() > 2) minArea = std::min(minArea, compute_area(polygons_[2]));
  if (polygons_.size() > 3) minArea = std::min(minArea, compute_area(polygons_[3]));
  if (polygons_.size() > 4) minArea = std::min(minArea, compute_area(polygons_[4]));

  std::cout << std::fixed << std::setprecision(1) << minArea << "\n";
}

void CommandProcessor::command_intersections(const std::string& rest) const {
  if (!rest.empty()) throw std::runtime_error("Unexpected argument");

  int count = 0;
  if (polygons_.size() > 1 && polygons_intersect(polygons_[0], polygons_[1])) ++count;
  if (polygons_.size() > 2 && polygons_intersect(polygons_[0], polygons_[2])) ++count;
  if (polygons_.size() > 3 && polygons_intersect(polygons_[0], polygons_[3])) ++count;
  if (polygons_.size() > 4 && polygons_intersect(polygons_[0], polygons_[4])) ++count;
  if (polygons_.size() > 2 && polygons_intersect(polygons_[1], polygons_[2])) ++count;
  if (polygons_.size() > 3 && polygons_intersect(polygons_[1], polygons_[3])) ++count;
  if (polygons_.size() > 4 && polygons_intersect(polygons_[1], polygons_[4])) ++count;
  if (polygons_.size() > 3 && polygons_intersect(polygons_[2], polygons_[3])) ++count;
  if (polygons_.size() > 4 && polygons_intersect(polygons_[2], polygons_[4])) ++count;
  if (polygons_.size() > 4 && polygons_intersect(polygons_[3], polygons_[4])) ++count;

  std::cout << count << "\n";
}

void CommandProcessor::command_rmecho(const std::string& rest) const {
  if (!rest.empty()) throw std::runtime_error("Unexpected argument");
  if (polygons_.empty()) {
    std::cout << "0\n";
    return;
  }

  Polygon last = polygons_.back();
  std::vector<Polygon> copy = polygons_;
  process_rmecho(copy, last);
}
}
