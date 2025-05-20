#include "get_area.hpp"
#include <numeric>
#include <algorithm>
#include <functional>
#include <io_fmt_guard.hpp>
#include "functors.hpp"

void maslevtsov::get_area(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::vector< Polygon > filtered;
  std::string subcommand;
  in >> subcommand;
  if (subcommand == "EVEN") {
    std::copy_if(polygons.cbegin(), polygons.cend(), std::back_inserter(filtered), is_even_vertex_num);
  } else if (subcommand == "ODD") {
    std::copy_if(polygons.cbegin(), polygons.cend(), std::back_inserter(filtered), is_odd_vertex_num);
  } else if (subcommand == "MEAN") {
    if (polygons.empty()) {
      throw std::invalid_argument("no polygons");
    }
    std::copy(polygons.cbegin(), polygons.cend(), std::back_inserter(filtered));
  } else {
    std::size_t vertex_num = std::stoull(subcommand);
    if (vertex_num < 3) {
      throw std::invalid_argument("invalid polygon");
    }
    auto same_vertex_num = std::bind(is_equal_vertex_num, vertex_num, std::placeholders::_1);
    std::copy_if(polygons.cbegin(), polygons.cend(), std::back_inserter(filtered), same_vertex_num);
  }
  std::vector< double > to_accumulate;
  std::transform(filtered.cbegin(), filtered.cend(), std::back_inserter(to_accumulate), get_polygon_area);
  double result = std::accumulate(to_accumulate.cbegin(), to_accumulate.cend(), 0.0);
  if (subcommand == "MEAN") {
    result /= filtered.size();
  }
  maslevtsov::IOFmtGuard guard(out);
  out << std::fixed << std::setprecision(1) << result;
}
