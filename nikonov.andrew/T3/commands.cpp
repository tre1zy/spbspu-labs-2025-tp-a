#include "commands.hpp"
#include <iomanip>
#include <numeric>
namespace nikonov
{
  namespace
  {
    struct IoGuard
    {
      std::ostream& out;
      explicit IoGuard(std::ostream& o) : out(o)
      {
        out << std::fixed << std::setprecision(1);
      }
      ~IoGuard()
      {
        out << std::defaultfloat;
      }
    };

    bool isEven(const nikonov::Polygon& poly)
    {
      return poly.points.size() % 2 == 0;
    }
    bool isOdd(const nikonov::Polygon& poly)
    {
      return !isEven(poly);
    }
    bool hasVertexCount(const nikonov::Polygon& poly, size_t count)
    {
      return poly.points.size() == count;
    }
  }
  void getArea(const std::vector< Polygon >& data, std::istream& in, std::ostream& out)
  {
    std::string subcommand;
    in >> subcommand;
    if (subcommand == "EVEN")
    {
      area::getEvenArea(data, out);
    }
    else if (subcommand == "ODD")
    {
      area::getOddArea(data, out);
    }
    else if (subcommand == "MEAN")
    {
      area::getMeanArea(data, out);
    }
    else
    {
      try
      {
        size_t cnt = std::stoul(subcommand);
        if (cnt < 3)
        {
          throw std::invalid_argument("Vertex count must be at least 3");
        }
        area::getVertexCountArea(data, cnt, out);
      }
      catch (const std::exception&)
      {
        out << "<INVALID COMMAND>\n";
      }
    }
  }
  namespace area
  {
    void getEvenArea(const std::vector< Polygon >& data, std::ostream& out)
    {
      //
    }
    void getOddArea(const std::vector< Polygon >& data, std::ostream& out)
    {
      //
    }
    void getMeanArea(const std::vector< Polygon >& data, std::ostream& out)
    {
      //
    }
    void getVertexCountArea(const std::vector< Polygon >& data, std::size_t count, std::ostream& out)
    {
      //
    }
  }
}
