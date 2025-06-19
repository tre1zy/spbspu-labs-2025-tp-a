#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <functional>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include "polygon.hpp"
#include "commands.hpp"

namespace amine
{
  namespace
  {
    void print_area(const std::vector<Polygon>& polygons, const std::function< bool(size_t) >& pred)
    {
      double sum = std::accumulate(
        polygons.begin(), polygons.end(), 0.0,
        [&](double acc, const Polygon& p) {
          return pred(p.vertexCount()) ? acc + compute_area(p) : acc;
        }
      );

      std::cout << sum << '\n';
    }

    void print_extreme_area(const std::vector<Polygon>& polygons, bool max)
    {
      auto it = max
        ? std::max_element(
            polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) {
              return compute_area(a) < compute_area(b);
            })
        : std::min_element(
            polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) {
              return compute_area(a) < compute_area(b);
            });

      std::cout << compute_area(*it) << '\n';
    }

    void print_extreme_vertices(const std::vector<Polygon>& polygons, bool max)
    {
      auto it = max
        ? std::max_element(
            polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) {
              return a.vertexCount() < b.vertexCount();
            })
        : std::min_element(
            polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) {
              return a.vertexCount() < b.vertexCount();
            });

      std::cout << it->vertexCount() << '\n';
    }
  }

  void execute_command(const std::string& command, const std::vector<std::string>& args, std::vector<Polygon>& polygons)
  {
    std::cout << std::fixed << std::setprecision(1);

    try {
      if (command == "AREA") {
        if (args.empty()) throw std::invalid_argument("Invalid command");

        if (args[0] == "EVEN") {
          print_area(polygons, [](size_t n) { return n % 2 == 0; });
        } else if (args[0] == "ODD") {
          print_area(polygons, [](size_t n) { return n % 2 == 1; });
        } else {
          throw std::invalid_argument("Invalid AREA argument");
        }
      } else if (command == "ECHO") {
        std::for_each(polygons.begin(), polygons.end(), [](const Polygon& p) {
          std::cout << p.vertexCount();
          std::for_each(p.points.begin(), p.points.end(), [](const Point& pt) {
            std::cout << " (" << pt.x << ";" << pt.y << ")";
          });
          std::cout << '\n';
        });
      } else if (command == "SAME") {
        std::cout << (std::adjacent_find(polygons.begin(), polygons.end()) != polygons.end() ? "YES" : "NO") << '\n';
      } else if (command == "RMECHO") {
        auto last = std::unique(polygons.begin(), polygons.end());
        polygons.erase(last, polygons.end());
      } else if (command == "INTERSECTIONS") {
        bool intersect = std::any_of(polygons.begin(), polygons.end(), [&](const Polygon& a) {
          return std::any_of(polygons.begin(), polygons.end(), [&](const Polygon& b) {
            return &a != &b && polygons_intersect(a, b);
          });
        });
        std::cout << (intersect ? "YES" : "NO") << '\n';
      } else if (command == "MAXAREA") {
        print_extreme_area(polygons, true);
      } else if (command == "MINAREA") {
        print_extreme_area(polygons, false);
      } else if (command == "MAXVERTEXES") {
        print_extreme_vertices(polygons, true);
      } else if (command == "MINVERTEXES") {
        print_extreme_vertices(polygons, false);
      } else {
        Polygon poly;
        if (parse_polygon(command + " " + std::accumulate(args.begin(), args.end(), std::string(),
            [](const std::string& a, const std::string& b) { return a + " " + b; }), poly)) {
          polygons.push_back(std::move(poly));
        } else {
          throw std::invalid_argument("Invalid polygon format");
        }
      }
    } catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
    }
  }

  void process_commands(std::vector<Polygon>& polygons)
  {
    std::vector< std::string > allTokens(
      std::istream_iterator< std::string >(std::cin),
      std::istream_iterator< std::string >()
    );

    std::vector< std::vector< std::string > > commandGroups;
    std::vector< std::string > currentCommand;

    std::for_each(
      allTokens.begin(),
      allTokens.end(),
      [&](const std::string& token) {
        if (token == "AREA" || token == "SAME" || token == "RMECHO" || token == "INTERSECTIONS"
            || token == "ECHO" || token == "MAXAREA" || token == "MINAREA"
            || token == "MAXVERTEXES" || token == "MINVERTEXES"
            || token == "RECTS" || token == "RIGHTSHAPES") {
          if (!currentCommand.empty()) {
            commandGroups.push_back(std::move(currentCommand));
            currentCommand = {};
          }
        }
        currentCommand.push_back(token);
      }
    );

    if (!currentCommand.empty()) {
      commandGroups.push_back(std::move(currentCommand));
    }

    std::for_each(
      commandGroups.begin(),
      commandGroups.end(),
      [&](const std::vector< std::string >& tokens) {
        if (!tokens.empty()) {
          const std::string& command = tokens.front();
          const std::vector< std::string > args(tokens.begin() + 1, tokens.end());
          execute_command(command, args, polygons);
        }
      }
    );
  }
}
