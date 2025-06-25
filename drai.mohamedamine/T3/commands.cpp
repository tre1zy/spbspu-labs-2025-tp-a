#include "commands.hpp"
#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <stdexcept>
#include <string>
#include "polygon.hpp"

namespace amine
{
  bool isEven(const Polygon& p)
  {
    return p.points.size() % 2 == 0;
  }

  bool isOdd(const Polygon& p)
  {
    return !isEven(p);
  }

  double areaIfEven(const Polygon& p)
  {
    return isEven(p) ? compute_area(p) : 0.0;
  }

  double areaIfOdd(const Polygon& p)
  {
    return isOdd(p) ? compute_area(p) : 0.0;
  }

  bool hasVertexCount(const Polygon& p, int count)
  {
    return static_cast<int>(p.points.size()) == count;
  }

  bool isPolygonEqual(const Polygon& a, const Polygon& b)
  {
    if (a.points.size() != b.points.size())
    {
      return false;
    }
    return std::equal(a.points.begin(), a.points.end(), b.points.begin(), [](const Point& x, const Point& y)
    {
      return x.x == y.x && x.y == y.y;
    });
  }

  void process_rmecho(std::vector< Polygon >& polygons, const Polygon& query)
  {
    auto pred = [&query](const Polygon& lhs, const Polygon& rhs)
    {
      return isPolygonEqual(lhs, query) && isPolygonEqual(rhs, query);
    };

    auto old_size = polygons.size();
    auto new_end = std::unique(polygons.begin(), polygons.end(), pred);
    polygons.erase(new_end, polygons.end());
    std::cout << (old_size - polygons.size()) << "\n";
  }

  int countIntersections(const std::vector< Polygon >& polygons, const Polygon& query)
  {
    return std::count_if(
      polygons.begin(), polygons.end(),
      [&query](const Polygon& p)
      {
        return polygons_intersect(p, query);
      }
    );
  }

  class CommandProcessor
  {
  public:
    CommandProcessor(std::vector< Polygon >& polygons):
      polygons_(polygons)
    {
      using namespace std::placeholders;
      commands_["AREA"] = std::bind(&CommandProcessor::area, this, _1);
      commands_["COUNT"] = std::bind(&CommandProcessor::count, this, _1);
      commands_["RMECHO"] = std::bind(&CommandProcessor::rmecho, this, _1);
      commands_["INTERSECTIONS"] = std::bind(&CommandProcessor::intersections, this, _1);
    }

    void execute(std::istream_iterator< std::string > begin, std::istream_iterator< std::string > end)
{
  std::vector< std::string > tokens(begin, end);

  std::for_each(tokens.begin(), tokens.end(), [&](const std::string& cmd)
  {
    static std::vector< std::string >::iterator it = tokens.begin();
    if (it == tokens.end())
      return;

    const std::string& current = *it++;
    auto found = commands_.find(current);
    if (found == commands_.end())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    try
    {
      found->second(it);
    }
    catch (const std::exception&)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  });
}

  private:
    std::vector< Polygon >& polygons_;
    std::map< std::string, std::function< void(std::vector< std::string >::iterator&) > > commands_;

    void area(std::vector< std::string >::iterator& it)
    {
      if (it == std::vector< std::string >::iterator{})
        throw std::invalid_argument("missing area param");

      const std::string& arg = *it++;
      if (arg == "EVEN")
      {
        double res = std::transform_reduce(
          polygons_.begin(), polygons_.end(), 0.0,
          std::plus<>(), areaIfEven
        );
        std::cout << std::fixed << std::setprecision(1) << res << "\n";
      }
      else if (arg == "ODD")
      {
        double res = std::transform_reduce(
          polygons_.begin(), polygons_.end(), 0.0,
          std::plus<>(), areaIfOdd
        );
        std::cout << std::fixed << std::setprecision(1) << res << "\n";
      }
      else
      {
        int count = std::stoi(arg);
        if (count < 3)
          throw std::invalid_argument("invalid vertex count");

        double res = 0.0;
        for (const auto& p : polygons_)
        {
          if (static_cast<int>(p.points.size()) == count)
          {
            res += compute_area(p);
          }
        }
        std::cout << std::fixed << std::setprecision(1) << res << "\n";
      }
    }

    void count(std::vector< std::string >::iterator& it)
    {
      if (it == std::vector< std::string >::iterator{})
        throw std::invalid_argument("missing count param");

      const std::string& arg = *it++;
      int result = 0;
      if (arg == "EVEN")
      {
        result = std::count_if(polygons_.begin(), polygons_.end(), isEven);
      }
      else if (arg == "ODD")
      {
        result = std::count_if(polygons_.begin(), polygons_.end(), isOdd);
      }
      else
      {
        int count = std::stoi(arg);
        if (count < 3)
          throw std::invalid_argument("invalid vertex count");

        result = std::count_if(polygons_.begin(), polygons_.end(),
          [count](const Polygon& p)
          {
            return hasVertexCount(p, count);
          }
        );
      }
      std::cout << result << "\n";
    }

    void rmecho(std::vector< std::string >::iterator& it)
    {
      std::string rest = std::accumulate(it, it + std::distance(it, it + polygons_.size()), std::string(),
        [](const std::string& a, const std::string& b)
        {
          return a + (a.empty() ? "" : " ") + b;
        });
      Polygon query;
      if (!parse_polygon(rest, query))
        throw std::invalid_argument("invalid polygon");

      process_rmecho(polygons_, query);
    }

    void intersections(std::vector< std::string >::iterator& it)
    {
      std::string rest = std::accumulate(it, it + std::distance(it, it + polygons_.size()), std::string(),
        [](const std::string& a, const std::string& b)
        {
          return a + (a.empty() ? "" : " ") + b;
        });
      Polygon query;
      if (!parse_polygon(rest, query))
        throw std::invalid_argument("invalid polygon");

      int result = countIntersections(polygons_, query);
      std::cout << result << "\n";
    }
  };

  void process_commands(std::vector< Polygon >& polygons)
  {
    std::vector< std::string > tokens(
      (std::istream_iterator< std::string >(std::cin)),
      std::istream_iterator< std::string >()
    );
    CommandProcessor proc(polygons);
    proc.execute(tokens.begin(), tokens.end());
  }
}
