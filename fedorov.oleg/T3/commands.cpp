#include "commands.hpp"

#include <iomanip>
#include <numeric>
#include <algorithm>
#include <functional>
#include <map>

#include "functional.hpp"
#include "format_guard.hpp"

namespace fedorov
{
  namespace
  {
    void outputAreaOdd(std::ostream &out, const std::vector< Polygon > &polygons)
    {
      double total = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaOddAccumulator());
      out << total << '\n';
    }

    void outputAreaEven(std::ostream &out, const std::vector< Polygon > &polygons)
    {
      double total = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaEvenAccumulator());
      out << total << '\n';
    }

    void outputAreaMean(std::ostream &out, const std::vector< Polygon > &polygons)
    {
      if (polygons.empty())
      {
        throw std::logic_error("No polygons");
      }
      double total = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaMeanAccumulator());
      out << total / polygons.size() << '\n';
    }

    void countOdd(std::ostream &out, const std::vector< Polygon > &polygons)
    {
      out << std::count_if(polygons.begin(), polygons.end(), CountOddPredicate()) << '\n';
    }

    void countEven(std::ostream &out, const std::vector< Polygon > &polygons)
    {
      out << std::count_if(polygons.begin(), polygons.end(), CountEvenPredicate()) << '\n';
    }
  }

  static bool isEOL(std::istream &in)
  {
    in >> std::ws;
    return in.eof() || in.peek() == '\n';
  }

  void maxArea(std::ostream &out, const std::vector< Polygon > &polys)
  {
    auto it = std::max_element(polys.begin(), polys.end(), AreaComparator());
    FormatGuard guard(out);
    out << std::fixed << std::setprecision(1) << calcPolygonArea(*it) << '\n';
  }

  void minArea(std::ostream &out, const std::vector< Polygon > &polys)
  {
    auto it = std::min_element(polys.begin(), polys.end(), AreaComparator());
    FormatGuard guard(out);
    out << std::fixed << std::setprecision(1) << calcPolygonArea(*it) << '\n';
  }

  void maxVertices(std::ostream &out, const std::vector< Polygon > &polys)
  {
    auto it = std::max_element(polys.begin(), polys.end(), VerticesComparator());
    out << it->points.size() << '\n';
  }

  void minVertices(std::ostream &out, const std::vector< Polygon > &polys)
  {
    auto it = std::min_element(polys.begin(), polys.end(), VerticesComparator());
    out << it->points.size() << '\n';
  }

  void maxCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polys)
  {
    if (polys.empty())
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    std::string param;
    if (!(in >> param) || !isEOL(in))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    using CommandFunction = std::function< void(std::ostream &, const std::vector< Polygon > &) >;
    std::map< std::string, CommandFunction > subcommands = {{"AREA", maxArea}, {"VERTEXES", maxVertices}};

    auto it = subcommands.find(param);
    if (it != subcommands.end())
    {
      it->second(out, polys);
    }
    else
    {
      out << "<INVALID COMMAND>\n";
    }
  }

  void minCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polys)
  {
    if (polys.empty())
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    std::string param;
    if (!(in >> param) || !isEOL(in))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    using CommandFunction = std::function< void(std::ostream &, const std::vector< Polygon > &) >;
    std::map< std::string, CommandFunction > subcommands = {{"AREA", minArea}, {"VERTEXES", minVertices}};

    auto it = subcommands.find(param);
    if (it != subcommands.end())
    {
      it->second(out, polys);
    }
    else
    {
      out << "<INVALID COMMAND>\n";
    }
  }

  void areaCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    std::string param;
    if (!(in >> param) || !isEOL(in))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    try
    {
      FormatGuard guard(out);
      out << std::fixed << std::setprecision(1);

      using CommandFunction = std::function< void() >;
      std::map< std::string, CommandFunction > subcommands;

      using namespace std::placeholders;
      subcommands["ODD"] = std::bind(outputAreaOdd, std::ref(out), std::cref(polygons));
      subcommands["EVEN"] = std::bind(outputAreaEven, std::ref(out), std::cref(polygons));
      subcommands["MEAN"] = std::bind(outputAreaMean, std::ref(out), std::cref(polygons));

      auto it = subcommands.find(param);
      if (it != subcommands.end())
      {
        it->second();
      }
      else
      {
        size_t num = std::stoul(param);
        if (num < 3)
        {
          throw std::invalid_argument("Invalid vertex count");
        }
        double total = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaNumAccumulator(num));
        out << total << '\n';
      }
    }
    catch (...)
    {
      out << "<INVALID COMMAND>\n";
    }
  }

  void lessAreaCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    Polygon target;
    if (!(in >> target) || !PolygonValidator()(target) || !isEOL(in))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    double threshold = calcPolygonArea(target);
    out << std::count_if(polygons.begin(), polygons.end(),
                         std::bind(std::less< double >(), std::bind(calcPolygonArea, std::placeholders::_1), threshold))
        << '\n';
  }

  void echoCommand(std::istream &in, std::ostream &out, std::vector< Polygon > &polygons)
  {
    Polygon target;
    if (!(in >> target) || !PolygonValidator()(target) || !isEOL(in))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    size_t count = std::count(polygons.begin(), polygons.end(), target);

    std::vector< Polygon > result;
    result.reserve(polygons.size() + count);

    std::copy(polygons.begin(), polygons.end(), std::back_inserter(result));
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(result),
                 std::bind(std::equal_to< Polygon >(), std::placeholders::_1, target));

    polygons.swap(result);
    out << count << '\n';
  }

  void countCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    std::string param;
    if (!(in >> param) || !isEOL(in))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    try
    {
      using CommandFunction = std::function< void() >;
      std::map< std::string, CommandFunction > subcommands;

      using namespace std::placeholders;
      subcommands["ODD"] = std::bind(countOdd, std::ref(out), std::cref(polygons));
      subcommands["EVEN"] = std::bind(countEven, std::ref(out), std::cref(polygons));

      auto it = subcommands.find(param);
      if (it != subcommands.end())
      {
        it->second();
      }
      else
      {
        size_t num = std::stoul(param);
        if (num < 3)
        {
          throw std::invalid_argument("Invalid vertex count");
        }
        out << std::count_if(polygons.begin(), polygons.end(), CountNumPredicate(num)) << '\n';
      }
    }
    catch (...)
    {
      out << "<INVALID COMMAND>\n";
    }
  }
}
