#include <functional>
#include <algorithm>

#include "commands.hpp"
#include "functional.hpp"
#include "polygon.hpp"
#include "detail_commands.hpp"

namespace fedorov
{
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
}
