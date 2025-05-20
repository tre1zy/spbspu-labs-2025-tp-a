#include "get_max_min.hpp"
#include <functional>
#include <io_fmt_guard.hpp>
#include "functors.hpp"

void maslevtsov::get_max(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  if (polygons.empty()) {
    throw std::invalid_argument("no polygons");
  }
  std::string subcommand;
  in >> subcommand;
  if (subcommand == "AREA") {
    std::vector< double > areas;
    std::transform(polygons.cbegin(), polygons.cend(), std::back_inserter(areas), get_polygon_area);
    maslevtsov::IOFmtGuard guard(out);
    out << std::fixed << std::setprecision(1) << *std::max_element(areas.cbegin(), areas.cend());
  } else if (subcommand == "VERTEXES") {
    out << std::max_element(polygons.cbegin(), polygons.cend(), compare_vertex_num_less)->points.size();
  } else {
    throw std::invalid_argument("invalid subcommand");
  }
}

void maslevtsov::get_min(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  if (polygons.empty()) {
    throw std::invalid_argument("no polygons");
  }
  std::string subcommand;
  in >> subcommand;
  if (subcommand == "AREA") {
    std::vector< double > areas;
    std::transform(polygons.cbegin(), polygons.cend(), std::back_inserter(areas), get_polygon_area);
    maslevtsov::IOFmtGuard guard(out);
    out << std::fixed << std::setprecision(1) << *std::min_element(areas.cbegin(), areas.cend());
  } else if (subcommand == "VERTEXES") {
    out << std::min_element(polygons.cbegin(), polygons.cend(), compare_vertex_num_less)->points.size();
  } else {
    throw std::invalid_argument("invalid subcommand");
  }
}