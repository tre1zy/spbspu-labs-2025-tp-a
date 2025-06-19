#include "commands.hpp"
#include "polygon.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <numeric>
#include <optional>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <memory>

namespace amine
{
  namespace
  {
    std::unique_ptr<Polygon> parse_polygon(const std::vector<std::string>& args)
    {
      if (args.size() < 7 || args.size() % 2 == 0)
      {
        return nullptr;
      }

      const std::string& tag = args[0];
      std::vector<Point> points;
      for (std::size_t i = 1; i < args.size(); i += 2)
      {
        std::istringstream x_stream(args[i]);
        std::istringstream y_stream(args[i + 1]);
        double x = 0.0;
        double y = 0.0;
        if (!(x_stream >> x) || !(y_stream >> y))
        {
          return nullptr;
        }
        points.push_back({x, y});
      }

      return std::make_unique<Polygon>(tag, std::move(points));
    }

    void echo(const std::vector<std::string>& args, std::vector<Polygon>& polygons)
    {
      auto poly = parse_polygon(args);
      if (!poly)
      {
        std::cout << "<INVALID POLYGON>" << '\n';
        return;
      }
      polygons.push_back(*poly);
    }

    void count_cmd(const std::vector<std::string>& args, const std::vector<Polygon>& polygons)
    {
      if (args.empty())
      {
        std::cout << polygons.size() << '\n';
        return;
      }

      const std::string& type = args[0];
      if (type == "EVEN")
      {
        std::cout << std::count_if(polygons.begin(), polygons.end(), [](const Polygon& p)
        {
          return p.vertexCount() % 2 == 0;
        }) << '\n';
      }
      else if (type == "ODD")
      {
        std::cout << std::count_if(polygons.begin(), polygons.end(), [](const Polygon& p)
        {
          return p.vertexCount() % 2 == 1;
        }) << '\n';
      }
      else if (type == "VERTEXES" && args.size() == 2)
      {
        std::istringstream iss(args[1]);
        std::size_t target = 0;
        if (!(iss >> target))
        {
          throw std::invalid_argument("Invalid argument");
        }
        std::cout << std::count_if(polygons.begin(), polygons.end(), [target](const Polygon& p)
        {
          return p.vertexCount() == target;
        }) << '\n';
      }
      else
      {
        throw std::invalid_argument("Invalid COUNT argument");
      }
    }

    void area_cmd(const std::vector<std::string>& args, const std::vector<Polygon>& polygons)
    {
      if (args.empty())
      {
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, [](double acc, const Polygon& p)
        {
          return acc + compute_area(p);
        });
        std::cout << sum << '\n';
        return;
      }

      const std::string& type = args[0];
      if (type == "EVEN")
      {
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, [](double acc, const Polygon& p)
        {
          return acc + (p.vertexCount() % 2 == 0 ? compute_area(p) : 0.0);
        });
        std::cout << sum << '\n';
      }
      else if (type == "ODD")
      {
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, [](double acc, const Polygon& p)
        {
          return acc + (p.vertexCount() % 2 == 1 ? compute_area(p) : 0.0);
        });
        std::cout << sum << '\n';
      }
      else if (type == "VERTEXES" && args.size() == 2)
      {
        std::istringstream iss(args[1]);
        std::size_t target = 0;
        if (!(iss >> target))
        {
          throw std::invalid_argument("Invalid argument");
        }
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, [target](double acc, const Polygon& p)
        {
          return acc + (p.vertexCount() == target ? compute_area(p) : 0.0);
        });
        std::cout << sum << '\n';
      }
      else
      {
        throw std::invalid_argument("Invalid AREA argument");
      }
    }

    void max_cmd(const std::vector<std::string>& args, const std::vector<Polygon>& polygons)
    {
      if (args.empty())
      {
        std::cout << "<INVALID COMMAND>" << '\n';
        return;
      }

      const std::string& type = args[0];
      if (type == "AREA")
      {
        auto max_it = std::max_element(polygons.begin(), polygons.end(), [](const Polygon& a, const Polygon& b)
        {
          return compute_area(a) < compute_area(b);
        });
        if (max_it != polygons.end())
        {
          std::cout << compute_area(*max_it) << '\n';
        }
        else
        {
          std::cout << 0.0 << '\n';
        }
      }
      else if (type == "VERTEXES")
      {
        auto max_it = std::max_element(polygons.begin(), polygons.end(), [](const Polygon& a, const Polygon& b)
        {
          return a.vertexCount() < b.vertexCount();
        });
        if (max_it != polygons.end())
        {
          std::cout << max_it->vertexCount() << '\n';
        }
        else
        {
          std::cout << 0 << '\n';
        }
      }
      else
      {
        std::cout << "<INVALID COMMAND>" << '\n';
      }
    }

    void same_cmd(const std::vector<std::string>& args, const std::vector<Polygon>& polygons)
    {
      auto p = parse_polygon(args);
      if (!p)
      {
        std::cout << "<INVALID POLYGON>" << '\n';
        return;
      }

      std::cout << std::count_if(polygons.begin(), polygons.end(), [&p](const Polygon& q)
      {
        return *p == q;
      }) << '\n';
    }
  } // namespace

  void process_commands(std::vector<Polygon>& polygons)
  {
    std::cout << std::fixed << std::setprecision(1);
    std::string input((std::istreambuf_iterator<char>(std::cin)), std::istreambuf_iterator<char>());
    std::istringstream stream(input);
    std::vector<std::string> lines;
    std::string line;
    std::generate_n(std::istream_iterator<std::string>(stream), 0, [&]()
    {
      return std::getline(stream, line) ? (lines.push_back(line), "") : "";
    });

    std::for_each(lines.begin(), lines.end(), [&](const std::string& line)
    {
      std::istringstream iss(line);
      std::vector<std::string> tokens(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>());

      if (tokens.empty())
      {
        return;
      }

      const std::string& command = tokens[0];
      std::vector<std::string> args(tokens.begin() + 1, tokens.end());

      try
      {
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
          max_cmd(args, polygons);
        }
        else if (command == "SAME")
        {
          same_cmd(args, polygons);
        }
        else
        {
          std::cout << "<INVALID COMMAND>" << '\n';
        }
      }
      catch (const std::exception&)
      {
        std::cout << "<INVALID COMMAND>" << '\n';
      }
    });
  }
}
