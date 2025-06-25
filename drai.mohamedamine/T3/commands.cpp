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
struct CopyAreaIfEven
{
  std::vector< double >& result;
  explicit CopyAreaIfEven(std::vector< double >& r): result(r) {}

  void operator()(const Polygon& p) const
  {
    if (p.points.size() % 2 == 0)
    {
      result.push_back(compute_area(p));
    }
  }
};

struct CopyAreaIfOdd
{
  std::vector< double >& result;
  explicit CopyAreaIfOdd(std::vector< double >& r): result(r) {}

  void operator()(const Polygon& p) const
  {
    if (p.points.size() % 2 != 0)
    {
      result.push_back(compute_area(p));
    }
  }
};

struct CopyAreaIfNum
{
  int n;
  std::vector< double >& result;

  CopyAreaIfNum(int count, std::vector< double >& r):
    n(count), result(r)
  {}

  void operator()(const Polygon& p) const
  {
    if (static_cast< int >(p.points.size()) == n)
    {
      result.push_back(compute_area(p));
    }
  }
};
struct SumDoubles
{
  double operator()(double lhs, double rhs) const
  {
    return lhs + rhs;
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
      std::string_view lineContent = line.content;
      std::size_t pos = lineContent.find_first_not_of(' ');
      if (pos == std::string::npos) {
        return;
      }
      lineContent.remove_prefix(pos);

      std::string cmd;
      iss >> cmd;
      bool printDouble = false;
      double dblResult = 0.0;
      int intResult = 0;
      if (cmd == "AREA")
      {
        std::string arg;
        iss >> arg;
      else if (arg == "EVEN")
      {
        std::vector< double > areas;
        areas.reserve(polygons.size());
        std::for_each(polygons.begin(), polygons.end(), CopyAreaIfEven(areas));
        dblResult = std::inner_product(areas.begin(), areas.end(), std::vector< double >(areas.size(), 1.0).begin(), 0.0);
        printDouble = true;
      }
      else if (arg == "ODD")
      {
        std::vector< double > areas;
        areas.reserve(polygons.size());
        std::for_each(polygons.begin(), polygons.end(), CopyAreaIfOdd(areas));
        dblResult = std::inner_product(areas.begin(), areas.end(), std::vector< double >(areas.size(), 1.0).begin(), 0.0);
        printDouble = true;
      }
      else
      {
        int num = std::stoi(arg);
        std::vector< double > areas;
        areas.reserve(polygons.size());
        std::for_each(polygons.begin(), polygons.end(), CopyAreaIfNum(num, areas));
        dblResult = std::inner_product(areas.begin(), areas.end(), std::vector< double >(areas.size(), 1.0).begin(), 0.0);
        printDouble = true;
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
                std::vector< double > areas;
                areas.reserve(polygons.size());
                std::for_each(polygons.begin(), polygons.end(), CopyAreaIfNum(num, areas));

                dblResult = std::inner_product(
                  areas.begin(), areas.end(),
                  std::vector< double >(areas.size(), 1.0).begin(),
                  0.0
                );

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
        std::string_view lineView = line.content;
        std::size_t pos = lineView.find(' ');
        if (pos == std::string_view::npos)
        {
          throw std::logic_error("INVALID COMMAND");
        }
        else
        {
          lineView.remove_prefix(pos + 1);
          Polygon query;
          if (!parse_polygon(std::string(lineView), query) || query.points.size() < 3)
          {
            throw std::logic_error("INVALID COMMAND");
          }
          else
          {
            struct IntersectsWith
            {
              const Polygon& query;
              explicit IntersectsWith(const Polygon& q): query(q) {}
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
        std::string_view lineView = line.content;
        std::size_t pos = lineView.find(' ');
        if (pos == std::string_view::npos)
        {
          throw std::logic_error("INVALID COMMAND");
        }
        else
        {
          lineView.remove_prefix(pos + 1);
          Polygon query;
          if (!parse_polygon(std::string(lineView), query))
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
      else if (printDouble)
      {
        std::cout << std::fixed << std::setprecision(1) << dblResult << "\n";
      }
      else
      {
        std::cout << intResult << "\n";
      }
      catch (const std::exception& e)
  {
    std::cout << "<INVALID COMMAND>\n";
  }
    };
  };
  void process_commands(std::vector< Polygon >& polygons)
  {
    std::for_each(std::istream_iterator< Line >(std::cin), std::istream_iterator< Line >(), CommandProcessor(polygons));
  }
};
}