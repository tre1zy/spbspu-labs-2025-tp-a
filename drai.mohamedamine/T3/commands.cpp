#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <functional>

#include "polygon.hpp"
#include "commands.hpp"

namespace amine
{
  void process_commands(std::vector<Polygon>& polygons)
  {
    std::istream_iterator<std::string> it(std::cin);
    std::istream_iterator<std::string> end;

    std::vector<std::string> commands(it, end);

    std::for_each(commands.begin(), commands.end(), [&](const std::string& command) {
      if (command == "AREA") {
        std::for_each(polygons.begin(), polygons.end(), [](const Polygon& p) {
          std::cout << compute_area(p) << "\n";
        });
      }

      if (command == "RECT") {
        std::for_each(polygons.begin(), polygons.end(), [](const Polygon& p) {
          std::cout << (is_rectangle(p) ? "<TRUE>\n" : "<FALSE>\n");
        });
      }

      if (command == "RMECHO") {
        polygons.erase(
          std::unique(polygons.begin(), polygons.end(), [](const Polygon& a, const Polygon& b) {
            return a.points == b.points;
          }),
          polygons.end()
        );
      }

      if (command == "INTERSECTIONS") {
        std::for_each(polygons.begin(), polygons.end(), [&](const Polygon& a) {
          std::for_each(polygons.begin(), polygons.end(), [&](const Polygon& b) {
            if (&a != &b && polygons_intersect(a, b)) {
              std::cout << "<TRUE>\n";
            }
          });
        });
      }
    });
  }
}
