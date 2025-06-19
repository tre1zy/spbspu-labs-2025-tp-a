#include "commands.hpp"
#include "polygon.hpp"

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
#include <optional>
#include <cmath>

namespace amine
{
  namespace
  {
    double compute_area(const Polygon& polygon)
    {
      const auto& pts = polygon.points;
      if (pts.size() < 3)
      {
        return 0.0;
      }

      double area = std::inner_product(
        pts.begin(), pts.end() - 1, pts.begin() + 1, 0.0,
        std::plus<>(),
        [](const Point& a, const Point& b) {
          return static_cast<double>(a.x) * b.y - static_cast<double>(b.x) * a.y;
        }
      );

      area += static_cast<double>(pts.back().x) * pts.front().y - static_cast<double>(pts.front().x) * pts.back().y;
      return std::abs(area) / 2.0;
    }

    std::optional<Polygon> parse_polygon(const std::vector<std::string>& args)
    {
      std::istringstream iss(std::accumulate(args.begin(), args.end(), std::string{},
        [](const std::string& acc, const std::string& s) {
          return acc + (acc.empty() ? "" : " ") + s;
        }
      ));

      Polygon p;
      if (!(iss >> p))
      {
        return std::nullopt;
      }

      return p;
    }

    void echo(const std::vector<std::string>& args, std::vector<Polygon>& polygons)
    {
      auto polygon = parse_polygon(args);
      if (!polygon)
      {
        std::cout << "<INVALID POLYGON>" << std::endl;
        return;
      }

      polygons.push_back(*polygon);
    }

    void count_cmd(const std::vector<std::string>& args, const std::vector<Polygon>& polygons)
    {
      if (args.empty())
      {
        std::cout << polygons.size() << std::endl;
        return;
      }

      if (args[0] == "EVEN")
      {
        std::cout << std::count_if(polygons.begin(), polygons.end(),
          [](const Polygon& p) {
            return p.vertexCount() % 2 == 0;
          }
        ) << std::endl;
      }
      else if (args[0] == "ODD")
      {
        std::cout << std::count_if(polygons.begin(), polygons.end(),
          [](const Polygon& p) {
            return p.vertexCount() % 2 != 0;
          }
        ) << std::endl;
      }
      else
      {
        std::cout << "<INVALID COMMAND>" << std::endl;
      }
    }

    void area_cmd(const std::vector<std::string>& args, const std::vector<Polygon>& polygons)
    {
      if (args.empty())
      {
        std::cout << "<INVALID COMMAND>" << std::endl;
        return;
      }

      if (args[0] == "EVEN")
      {
        double result = std::accumulate(polygons.begin(), polygons.end(), 0.0,
          [](double acc, const Polygon& p) {
            return acc + (p.vertexCount() % 2 == 0 ? compute_area(p) : 0.0);
          }
        );
        std::cout << std::fixed << std::setprecision(1) << result << std::endl;
      }
      else if (args[0] == "ODD")
      {
        double result = std::accumulate(polygons.begin(), polygons.end(), 0.0,
          [](double acc, const Polygon& p) {
            return acc + (p.vertexCount() % 2 != 0 ? compute_area(p) : 0.0);
          }
        );
        std::cout << std::fixed << std::setprecision(1) << result << std::endl;
      }
      else
      {
        std::cout << "<INVALID COMMAND>" << std::endl;
      }
    }

    void extreme_area(const std::vector<Polygon>& polygons, bool max)
    {
      if (polygons.empty())
      {
        return;
      }

      auto comparator = [](const Polygon& a, const Polygon& b)
      {
        return compute_area(a) < compute_area(b);
      };

      auto it = max ? std::max_element(polygons.begin(), polygons.end(), comparator)
                    : std::min_element(polygons.begin(), polygons.end(), comparator);

      std::cout << *it << std::endl;
    }

    void extreme_vertices(const std::vector<Polygon>& polygons, bool max)
    {
      if (polygons.empty())
      {
        return;
      }

      auto comparator = [](const Polygon& a, const Polygon& b)
      {
        return a.vertexCount() < b.vertexCount();
      };

      auto it = max ? std::max_element(polygons.begin(), polygons.end(), comparator)
                    : std::min_element(polygons.begin(), polygons.end(), comparator);

      std::cout << *it << std::endl;
    }

    void same_cmd(const std::vector<std::string>& args, const std::vector<Polygon>& polygons)
    {
      auto polygon = parse_polygon(args);
      if (!polygon)
      {
        std::cout << "<INVALID POLYGON>" << std::endl;
        return;
      }

      std::cout << std::count(polygons.begin(), polygons.end(), *polygon) << std::endl;
    }

    std::vector<std::string> tokenize_line(const std::string& line)
    {
      std::istringstream iss(line);
      return std::vector<std::string>(
        std::istream_iterator<std::string>(iss),
        std::istream_iterator<std::string>()
      );
    }

    void dispatch_command(const std::vector<std::string>& tokens, std::vector<Polygon>& polygons)
    {
      if (tokens.empty())
      {
        return;
      }

      const std::string& command = tokens[0];
      const std::vector<std::string> args(tokens.begin() + 1, tokens.end());

      if (command == "ECHO")
      {
        echo(args, polygons);
      }
      else if (command == "COUNT")
      {
        count_cmd(args, polygons);
      }
      else if (command == "AREA")
      {
        area_cmd(args, polygons);
      }
      else if (command == "MAX")
      {
        if (args.empty())
        {
          std::cout << "<INVALID COMMAND>" << std::endl;
        }
        else if (args[0] == "AREA")
        {
          extreme_area(polygons, true);
        }
        else if (args[0] == "VERTEXES")
        {
          extreme_vertices(polygons, true);
        }
        else
        {
          std::cout << "<INVALID COMMAND>" << std::endl;
        }
      }
      else if (command == "MIN")
      {
        if (args.empty())
        {
          std::cout << "<INVALID COMMAND>" << std::endl;
        }
        else if (args[0] == "AREA")
        {
          extreme_area(polygons, false);
        }
        else if (args[0] == "VERTEXES")
        {
          extreme_vertices(polygons, false);
        }
        else
        {
          std::cout << "<INVALID COMMAND>" << std::endl;
        }
      }
      else if (command == "SAME")
      {
        same_cmd(args, polygons);
      }
      else
      {
        std::cout << "<INVALID COMMAND>" << std::endl;
      }
    }
  }
  void process_commands(std::vector<Polygon>& polygons)
  {
    std::istreambuf_iterator<char> begin(std::cin), end;
    std::string input(begin, end);
    std::istringstream stream(input);

    std::for_each(
      std::istream_iterator<std::string>(stream),
      std::istream_iterator<std::string>(),
      [&](const std::string&) {
        static std::istringstream line_stream(input);
        static std::string line;
        if (std::getline(line_stream, line))
        {
          dispatch_command(tokenize_line(line), polygons);
        }
      }
    );
  }
}
