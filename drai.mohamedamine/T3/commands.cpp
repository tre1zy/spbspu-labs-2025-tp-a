#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <functional>
#include <iomanip>
#include <sstream>

#include "polygon.hpp"
#include "commands.hpp"

namespace amine
{
  void execute_command(const std::string& command, const std::vector<std::string>& args, std::vector<Polygon>& polygons)
  {
    if (command == "INTERSECTIONS") {
      if (args.empty() || polygons.empty()) {
        std::cout << "<INVALID COMMAND>" << std::endl;
        return;
      }

      Polygon param;
      std::string poly_str = std::accumulate(args.begin(), args.end(), std::string(),
        [](const std::string& a, const std::string& b) {
          return a + (a.empty() ? "" : " ") + b;
        });

      if (!parse_polygon(poly_str, param)) {
        std::cout << "<INVALID COMMAND>" << std::endl;
        return;
      }

      int count = std::count_if(polygons.begin(), polygons.end(),
        [&param](const Polygon& p) {
          return polygons_intersect(p, param);
        });

      std::cout << count << std::endl;
    }
    else if (command == "RMECHO") {
      if (args.empty() || polygons.empty()) {
        std::cout << "<INVALID COMMAND>" << std::endl;
        return;
      }

      Polygon param;
      std::string poly_str = std::accumulate(args.begin(), args.end(), std::string(),
        [](const std::string& a, const std::string& b) {
          return a + (a.empty() ? "" : " ") + b;
        });

      if (!parse_polygon(poly_str, param)) {
        std::cout << "<INVALID COMMAND>" << std::endl;
        return;
      }

      auto new_end = std::unique(polygons.begin(), polygons.end(),
        [&param](const Polygon& a, const Polygon& b) {
          return a.points == param.points && b.points == param.points;
        });
      int removed = std::distance(new_end, polygons.end());
      polygons.erase(new_end, polygons.end());
      std::cout << removed << std::endl;
    }
    else {
      std::cout << "<INVALID COMMAND>" << std::endl;
    }
  }

  void process_command_group(std::vector<std::string>::iterator& it,
                           std::vector<std::string>::iterator end,
                           std::vector<Polygon>& polygons)
  {
    if (it == end) return;

    std::string command = *it++;
    std::vector<std::string> args;

    auto next_cmd = std::find_if(it, end,
      [](const std::string& s) {
        return s == "AREA" || s == "MAX" || s == "MIN" || s == "COUNT" || 
               s == "INTERSECTIONS" || s == "RMECHO";
      });

    std::copy(it, next_cmd, std::back_inserter(args));
    it = next_cmd;

    execute_command(command, args, polygons);
  }

  void process_commands(std::vector<Polygon>& polygons)
  {
    std::vector<std::string> commands;
    std::copy(std::istream_iterator<std::string>(std::cin),
              std::istream_iterator<std::string>(),
              std::back_inserter(commands));

    auto it = commands.begin();
    std::function<void()> process = [&]() {
      if (it != commands.end()) {
        process_command_group(it, commands.end(), polygons);
        process();
      }
    };

    process();
  }
}
