#include "commands.hpp"

#include <iomanip>
#include <numeric>
#include <algorithm>
#include <functional>

#include "functional.hpp"

namespace fedorov
{
  void maxArea(std::ostream &out, const std::vector< Polygon > &polys)
  {
    if (polys.empty())
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    auto it = std::max_element(polys.begin(), polys.end(), AreaComparator());
    out << std::fixed << std::setprecision(1) << calcPolygonArea(*it) << '\n';
  }

  void minArea(std::ostream &out, const std::vector< Polygon > &polys)
  {
    if (polys.empty())
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    auto it = std::min_element(polys.begin(), polys.end(), AreaComparator());
    out << std::fixed << std::setprecision(1) << calcPolygonArea(*it) << '\n';
  }

  void maxVertices(std::ostream &out, const std::vector< Polygon > &polys)
  {
    if (polys.empty())
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    auto it = std::max_element(polys.begin(), polys.end(), VerticesComparator());
    out << it->points.size() << '\n';
  }

  void minVertices(std::ostream &out, const std::vector< Polygon > &polys)
  {
    if (polys.empty())
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    auto it = std::min_element(polys.begin(), polys.end(), VerticesComparator());
    out << it->points.size() << '\n';
  }

  void maxCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polys)
  {
    std::string param;
    if (!(in >> param))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    if (param == "AREA")
    {
      maxArea(out, polys);
    }
    else if (param == "VERTEXES")
    {
      maxVertices(out, polys);
    }
    else
    {
      out << "<INVALID COMMAND>\n";
    }
  }

  void minCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polys)
  {
    std::string param;
    if (!(in >> param))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    if (param == "AREA")
    {
      minArea(out, polys);
    }
    else if (param == "VERTEXES")
    {
      minVertices(out, polys);
    }
    else
    {
      out << "<INVALID COMMAND>\n";
    }
  }

  void areaCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    std::string param;
    if (!(in >> param))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    try
    {
      if (param == "ODD")
      {
        double total = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaOddAccumulator());
        out << std::fixed << std::setprecision(1) << total << '\n';
      }
      else if (param == "EVEN")
      {
        double total = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaEvenAccumulator());
        out << std::fixed << std::setprecision(1) << total << '\n';
      }
      else if (param == "MEAN")
      {
        if (polygons.empty())
        {
          throw std::logic_error("No polygons");
        }
        double total = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaMeanAccumulator());
        out << std::fixed << std::setprecision(1) << total / polygons.size() << '\n';
      }
      else
      {
        size_t num = std::stoul(param);
        if (num < 3)
        {
          throw std::invalid_argument("Invalid vertex count");
        }
        double total = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaNumAccumulator(num));
        out << std::fixed << std::setprecision(1) << total << '\n';
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
    if (!(in >> target) || !PolygonValidator()(target))
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
    if (!(in >> target) || !PolygonValidator()(target))
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
    if (!(in >> param))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    try
    {
      if (param == "ODD")
      {
        out << std::count_if(polygons.begin(), polygons.end(), CountOddPredicate()) << '\n';
      }
      else if (param == "EVEN")
      {
        out << std::count_if(polygons.begin(), polygons.end(), CountEvenPredicate()) << '\n';
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
