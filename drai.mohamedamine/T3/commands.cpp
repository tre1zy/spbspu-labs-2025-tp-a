#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include "polygon.hpp"
#include "commands.hpp"

namespace amine {

namespace {

std::string join_args(const std::vector<std::string>& args)
{
  return std::accumulate(
    args.begin(),
    args.end(),
    std::string(),
    [](const std::string& a, const std::string& b) {
      return a.empty() ? b : a + " " + b;
    }
  );
}

template < typename F >
void print_count(const std::vector<Polygon>& polygons, F&& pred)
{
  std::cout << std::count_if(
    polygons.begin(),
    polygons.end(),
    std::forward<F>(pred)
  ) << '\n';
}

template < typename F >
void print_sum(const std::vector<Polygon>& polygons, F&& pred)
{
  double sum = std::accumulate(
    polygons.begin(),
    polygons.end(),
    0.0,
    [pred = std::forward<F>(pred)](double acc, const Polygon& p) {
      return acc + (pred(p) ? compute_area(p) : 0.0);
    }
  );
  std::cout << sum << '\n';
}

void print_extreme_area(const std::vector<Polygon>& polygons, bool max)
{
  const auto it = (max ? std::max_element : std::min_element)(
    polygons.begin(),
    polygons.end(),
    [](const Polygon& a, const Polygon& b) {
      return compute_area(a) < compute_area(b);
    }
  );
  std::cout << compute_area(*it) << '\n';
}

void print_extreme_vertices(const std::vector<Polygon>& polygons, bool max)
{
  const auto it = (max ? std::max_element : std::min_element)(
    polygons.begin(),
    polygons.end(),
    [](const Polygon& a, const Polygon& b) {
      return a.vertexCount() < b.vertexCount();
    }
  );
  std::cout << it->vertexCount() << '\n';
}

Polygon extract_polygon(const std::vector<std::string>& args)
{
  Polygon result{};
  const std::string str = join_args(args);
  if (!parse_polygon(str, result)) {
    throw std::invalid_argument("Invalid command");
  }
  return result;
}

}

void execute_command(
  const std::string& command,
  const std::vector<std::string>& args,
  std::vector<Polygon>& polygons
)
{
  std::cout << std::fixed << std::setprecision(1);

  try {
    if (command == "AREA") {
      if (args.empty()) {
        throw std::invalid_argument("Invalid command");
      }

      if (args[0] == "EVEN") {
        print_sum(polygons, [](const Polygon& p) {
          return p.vertexCount() % 2 == 0;
        });

      } else if (args[0] == "ODD") {
        print_sum(polygons, [](const Polygon& p) {
          return p.vertexCount() % 2 != 0;
        });

      } else if (args[0] == "MEAN") {
        if (polygons.empty()) {
          throw std::invalid_argument("Invalid command");
        }

        double total = std::accumulate(
          polygons.begin(),
          polygons.end(),
          0.0,
          [](double acc, const Polygon& p) {
            return acc + compute_area(p);
          }
        );
        std::cout << total / polygons.size() << '\n';

      } else {
        const size_t count = std::stoul(args[0]);
        if (count < 3) {
          throw std::invalid_argument("Invalid command");
        }

        print_sum(polygons, [count](const Polygon& p) {
          return p.vertexCount() == count;
        });
      }

    } else if (command == "COUNT") {
      if (args.empty()) {
        throw std::invalid_argument("Invalid command");
      }

      if (args[0] == "EVEN") {
        print_count(polygons, [](const Polygon& p) {
          return p.vertexCount() % 2 == 0;
        });

      } else if (args[0] == "ODD") {
        print_count(polygons, [](const Polygon& p) {
          return p.vertexCount() % 2 != 0;
        });

      } else {
        const size_t count = std::stoul(args[0]);
        if (count < 3) {
          throw std::invalid_argument("Invalid command");
        }

        print_count(polygons, [count](const Polygon& p) {
          return p.vertexCount() == count;
        });
      }

    } else if (command == "MAX") {
      if (args.empty() || polygons.empty()) {
        throw std::invalid_argument("Invalid command");
      }

      if (args[0] == "AREA") {
        print_extreme_area(polygons, true);

      } else if (args[0] == "VERTEXES") {
        print_extreme_vertices(polygons, true);

      } else {
        throw std::invalid_argument("Invalid command");
      }

    } else if (command == "MIN") {
      if (args.empty() || polygons.empty()) {
        throw std::invalid_argument("Invalid command");
      }

      if (args[0] == "AREA") {
        print_extreme_area(polygons, false);

      } else if (args[0] == "VERTEXES") {
        print_extreme_vertices(polygons, false);

      } else {
        throw std::invalid_argument("Invalid command");
      }

    } else if (command == "INTERSECTIONS") {
      const Polygon target = extract_polygon(args);
      const int result = std::count_if(
        polygons.begin(),
        polygons.end(),
        [&target](const Polygon& p) {
          return polygons_intersect(p, target);
        }
      );
      std::cout << result << '\n';

    } else if (command == "RMECHO") {
      const Polygon target = extract_polygon(args);

      const auto new_end = std::unique(
        polygons.begin(),
        polygons.end(),
        [&target](const Polygon& a, const Polygon& b) {
          return a == target && b == target;
        }
      );

      const int removed = std::distance(new_end, polygons.end());
      polygons.erase(new_end, polygons.end());
      std::cout << removed << '\n';

    } else {
      throw std::invalid_argument("Invalid command");
    }

  } catch (...) {
    std::cout << "<INVALID COMMAND>\n";
  }
}

void process_commands(std::vector<Polygon>& polygons)
{
  std::string line;

  std::generate_n(
    std::istream_iterator<std::string>(std::cin),
    std::numeric_limits<std::streamsize>::max(),
    []() { return ""; }
  ); // no-op for EOF

  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }

    std::istringstream iss(line);
    std::vector<std::string> tokens(
      std::istream_iterator<std::string>(iss),
      std::istream_iterator<std::string>()
    );

    if (tokens.empty()) {
      continue;
    }

    const std::string command = tokens.front();
    const std::vector<std::string> args(tokens.begin() + 1, tokens.end());

    execute_command(command, args, polygons);
  }
}

}
