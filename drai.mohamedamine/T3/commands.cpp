#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <iomanip>

#include "polygon.hpp"

namespace amine {

void process_commands(std::vector<Polygon>& polygons)
{
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(std::cin, line)) {
    lines.push_back(line);
  }

  std::for_each(lines.begin(), lines.end(), [&](const std::string& line) {
    std::istringstream iss(line);
    std::string command;
    iss >> command;

    if (command == "INTERSECTIONS") {
      std::string rest;
      std::getline(iss, rest);
      Polygon query;
      if (!parse_polygon(rest, query)) {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }
      int count = std::count_if(
        polygons.begin(), polygons.end(),
        [&](const Polygon& p) {
          return polygons_intersect(p, query);
        });
      std::cout << count << "\n";
    } else if (command == "RMECHO") {
      std::string rest;
      std::getline(iss, rest);
      Polygon query;
      if (!parse_polygon(rest, query)) {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      std::size_t old_size = polygons.size();
      polygons.erase(
        std::unique(polygons.begin(), polygons.end(),
                    [&](const Polygon& a, const Polygon& b) {
                      return a.points == b.points && a.points == query.points;
                    }),
        polygons.end());
      std::size_t new_size = polygons.size();
      std::cout << (old_size - new_size) << "\n";
    } else {
      std::cout << "<INVALID COMMAND>\n";
    }
  });
}

}
