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

    void operator()(const std::string& line) const
    {
      const std::string& content = line;
      if (content.empty()) {
       return;
      }

      std::string::size_type spacePos = content.find(' ');
      std::string cmd;
      std::string rest;

    if (spacePos == std::string::npos) {
     cmd = content;
   } else {
    cmd = content.substr(0, spacePos);
    rest = content.substr(spacePos + 1);
   }

try {
      bool printDouble = false;
      double dblResult = 0.0;
      int intResult = 0;
            if (cmd == "AREA")
      {
        std::string arg;
        std::string::size_type spacePos = rest.find(' ');

        if (spacePos == std::string::npos) {
          arg = rest;
        } else {
          arg = rest.substr(0, spacePos);
        }

        if (arg == "EVEN")
        {
          dblResult = 0.0;
          if (polygons.size() > 0 && polygons[0].points.size() % 2 == 0) {
            dblResult += compute_area(polygons[0]);
          }
          if (polygons.size() > 1 && polygons[1].points.size() % 2 == 0) {
            dblResult += compute_area(polygons[1]);
          }
          if (polygons.size() > 2 && polygons[2].points.size() % 2 == 0) {
            dblResult += compute_area(polygons[2]);
          }
          if (polygons.size() > 3 && polygons[3].points.size() % 2 == 0) {
            dblResult += compute_area(polygons[3]);
          }
          if (polygons.size() > 4 && polygons[4].points.size() % 2 == 0) {
            dblResult += compute_area(polygons[4]);
          }
          printDouble = true;
        }
        else if (arg == "ODD")
        {
          dblResult = 0.0;
          if (polygons.size() > 0 && polygons[0].points.size() % 2 != 0) {
            dblResult += compute_area(polygons[0]);
          }
          if (polygons.size() > 1 && polygons[1].points.size() % 2 != 0) {
            dblResult += compute_area(polygons[1]);
          }
          if (polygons.size() > 2 && polygons[2].points.size() % 2 != 0) {
            dblResult += compute_area(polygons[2]);
          }
          if (polygons.size() > 3 && polygons[3].points.size() % 2 != 0) {
            dblResult += compute_area(polygons[3]);
          }
          if (polygons.size() > 4 && polygons[4].points.size() % 2 != 0) {
            dblResult += compute_area(polygons[4]);
          }
          printDouble = true;
        }
        else if (arg == "MEAN")
        {
          if (polygons.empty()) {
            throw std::runtime_error("Invalid command");
          } else {
            double total = 0.0;
            if (polygons.size() > 0) total += compute_area(polygons[0]);
            if (polygons.size() > 1) total += compute_area(polygons[1]);
            if (polygons.size() > 2) total += compute_area(polygons[2]);
            if (polygons.size() > 3) total += compute_area(polygons[3]);
            if (polygons.size() > 4) total += compute_area(polygons[4]);

            dblResult = total / polygons.size();
            printDouble = true;
          }
        }
        else
        {
          bool isNum = !arg.empty() && std::all_of(arg.begin(), arg.end(), ::isdigit);
          if (isNum)
          {
            int num = std::stoi(arg);
            if (num < 3)
            {
              throw std::runtime_error("Invalid command");
            }
            else
            {
              dblResult = 0.0;
              if (polygons.size() > 0 && static_cast<int>(polygons[0].points.size()) == num) {
                dblResult += compute_area(polygons[0]);
              }
              if (polygons.size() > 1 && static_cast<int>(polygons[1].points.size()) == num) {
                dblResult += compute_area(polygons[1]);
              }
              if (polygons.size() > 2 && static_cast<int>(polygons[2].points.size()) == num) {
                dblResult += compute_area(polygons[2]);
              }
              if (polygons.size() > 3 && static_cast<int>(polygons[3].points.size()) == num) {
                dblResult += compute_area(polygons[3]);
              }
              if (polygons.size() > 4 && static_cast<int>(polygons[4].points.size()) == num) {
                dblResult += compute_area(polygons[4]);
              }
              printDouble = true;
            }
          }
          else
          {
            throw std::runtime_error("Invalid command");
          }
        }
      }

      else if (cmd == "MAX")
      {
        std::string arg;
        std::string::size_type argPos = rest.find(' ');
        if (argPos == std::string::npos) {
         arg = rest;
        } else {
         arg = rest.substr(0, argPos);
        }

        if (polygons.empty())
        {
          throw std::runtime_error("Invalid command");
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
          throw std::runtime_error("Invalid command");
        }
      }
      else if (cmd == "MIN")
      {
        std::string arg;
        std::string::size_type argPos = rest.find(' ');
        if (argPos == std::string::npos) {
          arg = rest;
        } else {
          arg = rest.substr(0, argPos);
        }

        if (polygons.empty())
        {
          throw std::runtime_error("Invalid command");
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
          throw std::runtime_error("Invalid command");
        }
      }
      else if (cmd == "COUNT")
      {
        std::string arg;
        std::string::size_type argPos = rest.find(' ');
        if (argPos == std::string::npos) {
          arg = rest;
        } else {
          arg = rest.substr(0, argPos);
        }

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
              throw std::runtime_error("Invalid command");
            }
            else
            {
              intResult = std::count_if(polygons.begin(), polygons.end(), CountNum(num));
            }
          }
          else
          {
            throw std::runtime_error("Invalid command");
          }
        }
      }
      else if (cmd == "INTERSECTIONS")
      {
        std::string rest = line.substr(spacePos + 1);
        if (rest.empty())
        {
          throw std::runtime_error("Invalid command");
        }
        else
        {
          rest.erase(0, rest.find_first_not_of(' '));
          Polygon query;
          if (!parse_polygon(rest, query) || query.points.size() < 3)
          {
            throw std::runtime_error("Invalid command");
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
        std::string rest = line.substr(spacePos + 1);
        if (rest.empty())
        {
          throw std::runtime_error("Invalid command");
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
            throw std::runtime_error("Invalid command");
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
        throw std::runtime_error("Invalid command");
      }
      if (printDouble) {
      std::cout << std::fixed << std::setprecision(1) << dblResult << "\n";
    } else {
      std::cout << intResult << "\n";
    }

  } catch (const std::exception&) {
    std::cout << "<INVALID COMMAND>\n";
     }
    };
  };
}
