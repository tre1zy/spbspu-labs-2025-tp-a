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
  struct AreaEvenAccumulator
  {
    double operator()(double acc, const Polygon& p) const
    {
      return acc + ((p.points.size() % 2 == 0) ? compute_area(p) : 0.0);
    }
  };

  struct AreaOddAccumulator
  {
    double operator()(double acc, const Polygon& p) const
    {
      return acc + ((p.points.size() % 2 != 0) ? compute_area(p) : 0.0);
    }
  };

  struct AreaMeanAccumulator
  {
    double operator()(double acc, const Polygon& p) const
    {
      return acc + compute_area(p);
    }
  };

  struct AreaNumAccumulator
  {
    int num;
    explicit AreaNumAccumulator(int n):
      num(n)
    {}
    double operator()(double acc, const Polygon& p) const
    {
      return acc + (static_cast< int >(p.points.size()) == num ? compute_area(p) : 0.0);
    }
  };

  struct CountEven
  {
    bool operator()(const Polygon& p) const
    {
      return p.points.size() % 2 == 0;
    }
  };

  struct CountOdd
  {
    bool operator()(const Polygon& p) const
    {
      return p.points.size() % 2 != 0;
    }
  };

  struct CountNum
  {
    int num;
    explicit CountNum(int n):
      num(n)
    {}
    bool operator()(const Polygon& p) const
    {
      return static_cast< int >(p.points.size()) == num;
    }
  };

  struct AreaComparator
  {
    bool operator()(const Polygon& a, const Polygon& b) const
    {
      return compute_area(a) < compute_area(b);
    }
  };

  struct VertexesComparator
  {
    bool operator()(const Polygon& a, const Polygon& b) const
    {
      return a.points.size() < b.points.size();
    }
  };
  std::istream& operator>>(std::istream& is, Line& line)
  {
    std::getline(is, line.content);
    return is;
  }

  struct EqualToQuery
  {
    const Polygon& query;
    explicit EqualToQuery(const Polygon& q):
      query(q)
    {}

    bool operator()(const Polygon& p) const
    {
      if (p.points.size() != query.points.size())
      {
        return false;
      }

      struct PointEqual
      {
        bool operator()(const Point& a, const Point& b) const
        {
          return a.x == b.x && a.y == b.y;
        }
      };

      return std::equal(p.points.begin(), p.points.end(), query.points.begin(), PointEqual());
    }
  };

  struct UniqueChecker
  {
    EqualToQuery eq;
    explicit UniqueChecker(const Polygon& q):
      eq(q)
    {}

    bool operator()(const Polygon& a, const Polygon& b) const
    {
      return eq(a) && eq(b);
    }
  };

  void process_rmecho(std::vector< Polygon >& polygons, const Polygon& query)
  {
    EqualToQuery equal_to_query(query);
    UniqueChecker unique_checker(query);

    size_t initial_size = polygons.size();
    auto new_end = std::unique(polygons.begin(), polygons.end(), unique_checker);
    polygons.erase(new_end, polygons.end());
    std::cout << (initial_size - polygons.size()) << "\n";
  }

  struct CommandProcessor
  {
    std::vector< Polygon >& polygons;

    explicit CommandProcessor(std::vector< Polygon >& polys):
      polygons(polys)
    {}

    void operator()(const Line& line) const
    {
      try
      {
      if (line.content.empty())
      {
        return;
      }
      std::istringstream iss(line.content);
      std::string cmd;
      iss >> cmd;
      bool printDouble = false;
      double dblResult = 0.0;
      int intResult = 0;
      if (cmd == "AREA")
      {
        std::string arg;
        iss >> arg;
        if (arg == "EVEN")
        {
          dblResult = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaEvenAccumulator());
          printDouble = true;
        }
        else if (arg == "ODD")
        {
          dblResult = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaOddAccumulator());
          printDouble = true;
        }
        else if (arg == "MEAN")
        {
          if (polygons.empty())
          {
            throw std::logic_error("INVALID COMMAND");
          }
          else
          {
            double total = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaMeanAccumulator());
            dblResult = total / polygons.size();
            printDouble = true;
          }
        }
        else
        {
          bool is_num = !arg.empty() && std::all_of(arg.begin(), arg.end(), ::isdigit);
          if (is_num)
          {
            int num = std::stoi(arg);
            if (num < 3)
            {
              throw std::logic_error("INVALID COMMAND");
            }
            else
            {
              dblResult = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaNumAccumulator(num));
              printDouble = true;
            }
          }
          else
          {
            throw std::logic_error("INVALID COMMAND");
          }
        }
      }
      else if (cmd == "MAX")
      {
        std::string arg;
        iss >> arg;
        if (polygons.empty())
        {
          throw std::logic_error("INVALID COMMAND");
        }
        else if (arg == "AREA")
        {
          auto it = std::max_element(polygons.begin(), polygons.end(), AreaComparator());
          dblResult = compute_area(*it);
          printDouble = true;
        }
        else if (arg == "VERTEXES")
        {
          auto it = std::max_element(polygons.begin(), polygons.end(), VertexesComparator());
          intResult = it->points.size();
        }
        else
        {
          throw std::logic_error("INVALID COMMAND");
        }
      }
      else if (cmd == "MIN")
      {
        std::string arg;
        iss >> arg;
        if (polygons.empty())
        {
          throw std::logic_error("INVALID COMMAND");
        }
        else if (arg == "AREA")
        {
          auto it = std::min_element(polygons.begin(), polygons.end(), AreaComparator());
          dblResult = compute_area(*it);
          printDouble = true;
        }
        else if (arg == "VERTEXES")
        {
          auto it = std::min_element(polygons.begin(), polygons.end(), VertexesComparator());
          intResult = it->points.size();
        }
        else
        {
          throw std::logic_error("INVALID COMMAND");
        }
      }
      else if (cmd == "COUNT")
      {
        std::string arg;
        iss >> arg;
        if (arg == "EVEN")
        {
          intResult = std::count_if(polygons.begin(), polygons.end(), CountEven());
        }
        else if (arg == "ODD")
        {
          intResult = std::count_if(polygons.begin(), polygons.end(), CountOdd());
        }
        else
        {
          bool is_num = !arg.empty() && std::all_of(arg.begin(), arg.end(), ::isdigit);
          if (is_num)
          {
            int num = std::stoi(arg);
            if (num < 3)
            {
              throw std::logic_error("INVALID COMMAND");
            }
            else
            {
              intResult = std::count_if(polygons.begin(), polygons.end(), CountNum(num));
            }
          }
          else
          {
            throw std::logic_error("INVALID COMMAND");
          }
        }
      }
      else if (cmd == "INTERSECTIONS")
      {
        std::string rest;
        std::getline(iss, rest);
        if (rest.empty())
        {
          throw std::logic_error("INVALID COMMAND");
        }
        else
        {
          rest.erase(0, rest.find_first_not_of(' '));
          Polygon query;
          if (!parse_polygon(rest, query) || query.points.size() < 3)
          {
            throw std::logic_error("INVALID COMMAND");
          }
          else
          {
            struct IntersectsWith
            {
              const Polygon& query;
              explicit IntersectsWith(const Polygon& q):
                query(q)
              {}
              bool operator()(const Polygon& p) const
              {
                return polygons_intersect(p, query);
              }
            };
            intResult = polygons.empty() ? 0 : std::count_if(polygons.begin(), polygons.end(), IntersectsWith(query));
          }
        }
      }
      else if (cmd == "RMECHO")
      {
        std::string rest;
        std::getline(iss, rest);
        if (rest.empty())
        {
          throw std::logic_error("INVALID COMMAND");
        }
        else
        {
          size_t pos = rest.find_first_not_of(' ');
          if (pos != std::string::npos)
          {
            rest = rest.substr(pos);
          }
          Polygon query;
          if (!parse_polygon(rest, query))
          {
            throw std::logic_error("INVALID COMMAND");
          }
          else
          {
            EqualToQuery equal_to_query(query);
            UniqueChecker unique_checker(query);

            size_t initial_size = polygons.size();
            auto new_end = std::unique(polygons.begin(), polygons.end(), unique_checker);
            polygons.erase(new_end, polygons.end());
            std::cout << (initial_size - polygons.size()) << "\n";
            return;
          }
        }
      }
      else
      {
        throw std::logic_error("INVALID COMMAND");
      }
      if (printDouble)
      {
        std::cout << std::fixed << std::setprecision(1) << dblResult << "\n";
      }
     else
    {
      std::cout << intResult << "\n";
    }
  };
  catch (const std::exception&)
  {
    std::cout << "<INVALID COMMAND>\n";
  }
}
};
  void process_commands(std::vector< Polygon >& polygons)
  {
    std::for_each(std::istream_iterator< Line >(std::cin), std::istream_iterator< Line >(), CommandProcessor(polygons));
  }
}
