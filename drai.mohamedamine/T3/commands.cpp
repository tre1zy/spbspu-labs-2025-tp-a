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

  std::istream& operator>>(std::istream& is, Line& line)
  {
    std::getline(is, line.content);
    return is;
  }

  // Functors to replace lambdas
  struct PolygonEqual {
    const Polygon& query;
    bool operator()(const Polygon& p) const {
      return p.points.size() == query.points.size() &&
             std::equal(p.points.begin(), p.points.end(), query.points.begin());
    }
  };

  struct ConsecutivePolygonEqual {
    PolygonEqual eq;
    ConsecutivePolygonEqual(const Polygon& q) : eq(q) {}
    bool operator()(const Polygon& a, const Polygon& b) const {
      return eq(a) && eq(b);
    }
  };

  struct PolygonIntersects {
    const Polygon& query;
    bool operator()(const Polygon& p) const {
      return polygons_intersect(p, query);
    }
  };

  struct AreaEvenAccumulator {
    double operator()(double acc, const Polygon& p) const {
      return acc + ((p.points.size() % 2 == 0) ? compute_area(p) : 0.0);
    }
  };

  struct AreaOddAccumulator {
    double operator()(double acc, const Polygon& p) const {
      return acc + ((p.points.size() % 2 != 0) ? compute_area(p) : 0.0);
    }
  };

  struct AreaMeanAccumulator {
    double operator()(double acc, const Polygon& p) const {
      return acc + compute_area(p);
    }
  };

  struct AreaVertexCountAccumulator {
    int num;
    double operator()(double acc, const Polygon& p) const {
      return acc + (static_cast<int>(p.points.size()) == num ? compute_area(p) : 0.0);
    }
  };

  struct CountEven {
    bool operator()(const Polygon& p) const {
      return p.points.size() % 2 == 0;
    }
  };

  struct CountOdd {
    bool operator()(const Polygon& p) const {
      return p.points.size() % 2 != 0;
    }
  };

  struct CountVertex {
    int num;
    bool operator()(const Polygon& p) const {
      return static_cast<int>(p.points.size()) == num;
    }
  };

  struct CompareArea {
    bool operator()(const Polygon& a, const Polygon& b) const {
      return compute_area(a) < compute_area(b);
    }
  };

  struct CompareVertexCount {
    bool operator()(const Polygon& a, const Polygon& b) const {
      return a.points.size() < b.points.size();
    }
  };

  void process_rmecho(std::vector< Polygon >& polygons, const Polygon& query)
  {
    size_t initial_size = polygons.size();
    ConsecutivePolygonEqual consecutive_equal(query);
    auto new_end = std::unique(polygons.begin(), polygons.end(), consecutive_equal);
    polygons.erase(new_end, polygons.end());
    std::cout << (initial_size - polygons.size()) << "\n";
  }

  void process_commands(std::vector< Polygon >& polygons)
  {
    std::vector<Line> lines;
    std::copy(std::istream_iterator<Line>(std::cin), std::istream_iterator<Line>(), std::back_inserter(lines));

    for (const auto& line : lines) {
      if (line.content.empty()) continue;

      std::istringstream iss(line.content);
      std::string cmd;
      iss >> cmd;
      bool invalid = false;
      bool printDouble = false;
      double dblResult = 0.0;
      int intResult = 0;

      if (cmd == "AREA") {
        std::string arg;
        iss >> arg;
        if (arg == "EVEN") {
          dblResult = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaEvenAccumulator());
          printDouble = true;
        }
        else if (arg == "ODD") {
          dblResult = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaOddAccumulator());
          printDouble = true;
        }
        else if (arg == "MEAN") {
          if (polygons.empty()) {
            invalid = true;
          }
          else {
            double total = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaMeanAccumulator());
            dblResult = total / polygons.size();
            printDouble = true;
          }
        }
        else {
          bool is_num = !arg.empty() && std::all_of(arg.begin(), arg.end(), ::isdigit);
          if (is_num) {
            int num = std::stoi(arg);
            if (num < 3) {
              invalid = true;
            }
            else {
              dblResult = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaVertexCountAccumulator{num});
              printDouble = true;
            }
          }
          else {
            invalid = true;
          }
        }
      }
      else if (cmd == "MAX") {
        std::string arg;
        iss >> arg;
        if (polygons.empty()) {
          invalid = true;
        }
        else if (arg == "AREA") {
          auto it = std::max_element(polygons.begin(), polygons.end(), CompareArea());
          dblResult = compute_area(*it);
          printDouble = true;
        }
        else if (arg == "VERTEXES") {
          auto it = std::max_element(polygons.begin(), polygons.end(), CompareVertexCount());
          intResult = it->points.size();
        }
        else {
          invalid = true;
        }
      }
      else if (cmd == "MIN") {
        std::string arg;
        iss >> arg;
        if (polygons.empty()) {
          invalid = true;
        }
        else if (arg == "AREA") {
          auto it = std::min_element(polygons.begin(), polygons.end(), CompareArea());
          dblResult = compute_area(*it);
          printDouble = true;
        }
        else if (arg == "VERTEXES") {
          auto it = std::min_element(polygons.begin(), polygons.end(), CompareVertexCount());
          intResult = it->points.size();
        }
        else {
          invalid = true;
        }
      }
      else if (cmd == "COUNT") {
        std::string arg;
        iss >> arg;
        if (arg == "EVEN") {
          intResult = std::count_if(polygons.begin(), polygons.end(), CountEven());
        }
        else if (arg == "ODD") {
          intResult = std::count_if(polygons.begin(), polygons.end(), CountOdd());
        }
        else {
          bool is_num = !arg.empty() && std::all_of(arg.begin(), arg.end(), ::isdigit);
          if (is_num) {
            int num = std::stoi(arg);
            if (num < 3) {
              invalid = true;
            }
            else {
              intResult = std::count_if(polygons.begin(), polygons.end(), CountVertex{num});
            }
          }
          else {
            invalid = true;
          }
        }
      }
      else if (cmd == "INTERSECTIONS") {
        std::string rest;
        std::getline(iss, rest);
        if (rest.empty()) {
          invalid = true;
        }
        else {
          rest.erase(0, rest.find_first_not_of(' '));
          Polygon query;
          if (!parse_polygon(rest, query) || query.points.size() < 3) {
            invalid = true;
          }
          else {
            intResult = polygons.empty() ? 0 : std::count_if(polygons.begin(), polygons.end(), PolygonIntersects{query});
          }
        }
      }
      else if (cmd == "RMECHO") {
        std::string rest;
        std::getline(iss, rest);
        if (rest.empty()) {
          invalid = true;
        }
        else {
          size_t pos = rest.find_first_not_of(' ');
          if (pos != std::string::npos) {
            rest = rest.substr(pos);
          }
          Polygon query;
          if (!parse_polygon(rest, query)) {
            invalid = true;
          }
          else {
            process_rmecho(polygons, query);
            continue;
          }
        }
      }
      else {
        invalid = true;
      }

      if (!invalid && cmd != "INTERSECTIONS" && cmd != "RMECHO") {
        std::string extra;
        if (iss >> extra) {
          invalid = true;
        }
      }

      if (invalid) {
        std::cout << "<INVALID COMMAND>\n";
      }
      else if (printDouble) {
        std::cout << std::fixed << std::setprecision(1) << dblResult << "\n";
      }
      else {
        std::cout << intResult << "\n";
      }
    }
  }
}
