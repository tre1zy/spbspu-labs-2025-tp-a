#include "commands.hpp"

#include <iomanip>
#include <algorithm>

#include "format_guard.hpp"
#include "functional.hpp"
#include "detail_commands.hpp"

namespace fedorov
{

  bool isEOL(std::istream &in)
  {
    std::istream::sentry sentry(in, true);
    if (!sentry)
    {
      return true;
    }
    int c = in.peek();
    return c == '\n' || c == EOF;
  }
  void outputFormattedArea(double value, std::ostream &out)
  {
    FormatGuard guard(out);
    out << std::fixed << std::setprecision(1) << value << '\n';
  }

  void maxArea(std::ostream &out, const std::vector< Polygon > &polys)
  {
    auto it = std::max_element(polys.begin(), polys.end(), AreaComparator());
    outputFormattedArea(calcPolygonArea(*it), out);
  }

  void minArea(std::ostream &out, const std::vector< Polygon > &polys)
  {
    auto it = std::min_element(polys.begin(), polys.end(), AreaComparator());
    outputFormattedArea(calcPolygonArea(*it), out);
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
}
