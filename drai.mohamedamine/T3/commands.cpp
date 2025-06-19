#include "commands.hpp"
#include "polygon.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include <stdexcept>
#include <optional>
#include <sstream>

namespace amine
{
  namespace
  {
    using std::string;
    using std::vector;

    std::optional<Polygon> parse_polygon(const std::vector<std::string>& args);

    void count_cmd(const vector<string>& args, const vector<Polygon>& polys)
    {
      if (args.empty())
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      if (args[0] == "EVEN")
      {
        size_t count = std::count_if(polys.begin(), polys.end(), [](const Polygon& p)
        {
          return p.vertexCount() % 2 == 0;
        });
        std::cout << count << '\n';
      }
      else if (args[0] == "ODD")
      {
        size_t count = std::count_if(polys.begin(), polys.end(), [](const Polygon& p)
        {
          return p.vertexCount() % 2 == 1;
        });
        std::cout << count << '\n';
      }
      else
      {
        try
        {
          size_t target = std::stoull(args[0]);
          if (target < 3)
          {
            std::cout << "<INVALID COMMAND>\n";
            return;
          }
          size_t count = std::count_if(polys.begin(), polys.end(), [target](const Polygon& p)
          {
            return p.vertexCount() == target;
          });
          std::cout << count << '\n';
        }
        catch (...)
        {
          std::cout << "<INVALID COMMAND>\n";
        }
      }
    }

    void area_cmd(const vector<string>& args, const vector<Polygon>& polys)
    {
      if (args.empty())
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      if (args[0] == "EVEN")
      {
        double sum = std::accumulate(polys.begin(), polys.end(), 0.0, [](double acc, const Polygon& p)
        {
          return acc + (p.vertexCount() % 2 == 0 ? compute_area(p) : 0.0);
        });
        std::cout << sum << '\n';
      }
      else if (args[0] == "ODD")
      {
        double sum = std::accumulate(polys.begin(), polys.end(), 0.0, [](double acc, const Polygon& p)
        {
          return acc + (p.vertexCount() % 2 == 1 ? compute_area(p) : 0.0);
        });
        std::cout << sum << '\n';
      }
      else if (args[0] == "MEAN")
      {
        if (polys.empty())
        {
          std::cout << "<INVALID COMMAND>\n";
          return;
        }
        double sum = std::accumulate(polys.begin(), polys.end(), 0.0, [](double acc, const Polygon& p)
        {
          return acc + compute_area(p);
        });
        std::cout << sum / polys.size() << '\n';
      }
      else
      {
        try
        {
          size_t target = std::stoull(args[0]);
          if (target < 3)
          {
            std::cout << "<INVALID COMMAND>\n";
            return;
          }
          double sum = std::accumulate(polys.begin(), polys.end(), 0.0, [target](double acc, const Polygon& p)
          {
            return acc + (p.vertexCount() == target ? compute_area(p) : 0.0);
          });
          std::cout << sum << '\n';
        }
        catch (...)
        {
          std::cout << "<INVALID COMMAND>\n";
        }
      }
    }

    void max_cmd(const vector<string>& args, const vector<Polygon>& polys)
    {
      if (args.empty())
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      if (args[0] == "AREA")
      {
        if (polys.empty())
        {
          std::cout << "<INVALID COMMAND>\n";
          return;
        }
        auto max_it = std::max_element(polys.begin(), polys.end(), [](const Polygon& a, const Polygon& b)
        {
          return compute_area(a) < compute_area(b);
        });
        std::cout << compute_area(*max_it) << '\n';
      }
      else if (args[0] == "VERTEXES")
      {
        if (polys.empty())
        {
          std::cout << "<INVALID COMMAND>\n";
          return;
        }
        auto max_it = std::max_element(polys.begin(), polys.end(), [](const Polygon& a, const Polygon& b)
        {
          return a.vertexCount() < b.vertexCount();
        });
        std::cout << max_it->vertexCount() << '\n';
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
      }
    }

    void echo(const std::vector<std::string>& args, std::vector<Polygon>& polys)
    {
      auto p = parse_polygon(args);
      if (p)
      {
        polys.push_back(*p);
      }
    }

    void same_cmd(const std::vector<std::string>& args, const std::vector<Polygon>& polys)
    {
      auto p = parse_polygon(args);
      if (!p)
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      size_t count = std::count(polys.begin(), polys.end(), *p);
      std::cout << count << '\n';
    }
  }

  void execute_command(const std::string& command, const std::vector<std::string>& args, std::vector<Polygon>& polygons)
  {
    std::cout << std::fixed << std::setprecision(1);

    if (command == "COUNT")
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
    else if (command == "ECHO")
    {
      echo(args, polygons);
    }
    else if (command == "SAME")
    {
      same_cmd(args, polygons);
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
