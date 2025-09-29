#include "commands.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <numeric>
#include <algorithm>

namespace
{
  void printArea(double a)
  {
    std::cout << a << "\n";
  }

  void printCount(size_t c)
  {
    std::cout << c << "\n";
  }

  double accumulateArea(double sum, const cherkasov::Polygon& p)
  {
    return sum + cherkasov::getArea(p);
  }

  double accumulateEven(double sum, const cherkasov::Polygon& p)
  {
    return (p.points.size() % 2 == 0) ? sum + cherkasov::getArea(p) : sum;
  }

  double accumulateOdd(double sum, const cherkasov::Polygon& p)
  {
    return (p.points.size() % 2 == 1) ? sum + cherkasov::getArea(p) : sum;
  }

  double accumulateWithN(double sum, const cherkasov::Polygon& p, size_t n)
  {
    return (p.points.size() == n) ? sum + cherkasov::getArea(p) : sum;
  }

  bool lessArea(const cherkasov::Polygon& a, const cherkasov::Polygon& b)
  {
    return cherkasov::getArea(a) < cherkasov::getArea(b);
  }

  bool lessVertexes(const cherkasov::Polygon& a, const cherkasov::Polygon& b)
  {
    return a.points.size() < b.points.size();
  }

  bool isEvenPolygon(const cherkasov::Polygon& p)
  {
    return p.points.size() % 2 == 0;
  }

  bool isOddPolygon(const cherkasov::Polygon& p)
  {
    return p.points.size() % 2 == 1;
  }

  bool hasVertexCount(const cherkasov::Polygon& p, size_t n)
  {
    return p.points.size() == n;
  }

  struct IntersectsWith
  {
    const cherkasov::Polygon& q_;
    IntersectsWith(const cherkasov::Polygon& q):
    q_(q)
    {}
    bool operator()(const cherkasov::Polygon& p) const
    {
      return cherkasov::polygonsIntersect(p, q_);
    }
  };
}

namespace cherkasov
{
  void processCommand(const std::vector<Polygon>& polys, const std::string& cmd)
  {
    std::istringstream iss(cmd);
    std::string command;
    iss >> command;
    std::string arg;
    std::getline(iss, arg);

    if (!arg.empty() && arg[0] == ' ')
    {
      arg.erase(0, 1);
    }

    try
    {
      std::cout << std::fixed << std::setprecision(1);
      if (command == "AREA")
      {
        handleArea(polys, arg);
      }
      else if (command == "MAX")
      {
        handleMax(polys, arg);
      }
      else if (command == "MIN")
      {
        handleMin(polys, arg);
      }
      else if (command == "COUNT")
      {
        handleCount(polys, arg);
      }
      else if (command == "INTERSECTIONS")
      {
        handleIntersections(polys, arg);
      }
      else if (command == "RIGHTSHAPES")
      {
        handleRightShapes(polys);
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
      }
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  void handleArea(const std::vector<Polygon>& polys, const std::string& arg)
  {
    if (arg == "MEAN")
    {
      if (polys.empty())
      {
        throw std::invalid_argument("no polys");
      }
      double sum = std::accumulate(polys.begin(), polys.end(), 0.0, accumulateArea);
      printArea(sum / polys.size());
    }
    else if (arg == "EVEN")
    {
      double sum = std::accumulate(polys.begin(), polys.end(), 0.0, accumulateEven);
      printArea(sum);
    }
    else if (arg == "ODD")
    {
      double sum = std::accumulate(polys.begin(), polys.end(), 0.0, accumulateOdd);
      printArea(sum);
    }
    else
    {
      size_t num = std::stoul(arg);
      if (num < 3)
      {
        throw std::invalid_argument("invalid vertex count");
      }
      double sum = 0.0;
      for (const Polygon& p : polys)
      {
        sum = accumulateWithN(sum, p, num);
      }
      printArea(sum);
    }
  }

  void handleMax(const std::vector<Polygon>& polys, const std::string& arg)
  {
    if (polys.empty())
    {
      throw std::invalid_argument("no polys");
    }
    if (arg == "AREA")
    {
      auto it = std::max_element(polys.begin(), polys.end(), lessArea);
      printArea(getArea(*it));
    }
    else if (arg == "VERTEXES")
    {
      auto it = std::max_element(polys.begin(), polys.end(), lessVertexes);
      printCount(it->points.size());
    }
    else
    {
      throw std::invalid_argument("bad arg");
    }
  }

  void handleMin(const std::vector<Polygon>& polys, const std::string& arg)
  {
    if (polys.empty())
    {
      throw std::invalid_argument("no polys");
    }
    if (arg == "AREA")
    {
      auto it = std::min_element(polys.begin(), polys.end(), lessArea);
      printArea(getArea(*it));
    }
    else if (arg == "VERTEXES")
    {
      auto it = std::min_element(polys.begin(), polys.end(), lessVertexes);
      printCount(it->points.size());
    }
    else
    {
      throw std::invalid_argument("bad arg");
    }
  }

  void handleCount(const std::vector<Polygon>& polys, const std::string& arg)
  {
    if (arg == "EVEN")
    {
      size_t c = std::count_if(polys.begin(), polys.end(), isEvenPolygon);
      printCount(c);
    }
    else if (arg == "ODD")
    {
      size_t c = std::count_if(polys.begin(), polys.end(), isOddPolygon);
      printCount(c);
    }
    else
    {
      size_t num = std::stoul(arg);
      if (num < 3)
      {
        throw std::invalid_argument("invalid vertex count");
      }
      size_t c = 0;
      for (const Polygon& p : polys)
      {
        if (hasVertexCount(p, num))
        {
          ++c;
        }
      }
      printCount(c);
    }
  }

  void handleIntersections(const std::vector<Polygon>& polys, const std::string& arg)
  {
    Polygon q = parsePolygon(arg);
    size_t count = std::count_if(polys.begin(), polys.end(), IntersectsWith(q));
    printCount(count);
  }

  void handleRightShapes(const std::vector<Polygon>& polys)
  {
    size_t c = std::count_if(polys.begin(), polys.end(), hasRightAngle);
    printCount(c);
  }
}