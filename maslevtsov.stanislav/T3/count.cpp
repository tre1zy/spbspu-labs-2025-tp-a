#include "count.hpp"
#include <functional>
#include "functors.hpp"

void maslevtsov::count_vertexes(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::string subcommand;
  in >> subcommand;
  if (subcommand == "EVEN") {
    out << std::count_if(polygons.cbegin(), polygons.cend(), is_even_vertex_num);
  } else if (subcommand == "ODD") {
    out << std::count_if(polygons.cbegin(), polygons.cend(), is_odd_vertex_num);
  } else {
    std::size_t vertex_num = std::stoull(subcommand);
    if (vertex_num < 3) {
      throw std::invalid_argument("invalid polygon");
    }
    auto same_vertex_num = std::bind(is_equal_vertex_num, vertex_num, std::placeholders::_1);
    out << std::count_if(polygons.cbegin(), polygons.cend(), same_vertex_num);
  }
}
