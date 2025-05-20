#include "count.hpp"
#include <functional>
#include <algorithm>
#include <map>
#include "functors.hpp"

namespace {
  void count_even_vertexes(const std::vector< maslevtsov::Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    out << std::count_if(polygons.cbegin(), polygons.cend(), maslevtsov::is_even_vertex_num);
  }

  void count_odd_vertexes(const std::vector< maslevtsov::Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    out << std::count_if(polygons.cbegin(), polygons.cend(), maslevtsov::is_odd_vertex_num);
  }
}

void maslevtsov::count_vertexes(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::map< std::string, std::function< void(std::istream&, std::ostream&) > > subcommands;
  using namespace std::placeholders;
  subcommands["EVEN"] = std::bind(count_even_vertexes, std::cref(polygons), _1, _2);
  subcommands["ODD"] = std::bind(count_odd_vertexes, std::cref(polygons), _1, _2);

  std::string subcommand;
  in >> subcommand;
  try {
    subcommands.at(subcommand)(in, out);
  } catch (const std::out_of_range&) {
    std::size_t vertex_num = std::stoull(subcommand);
    if (vertex_num < 3) {
      throw std::invalid_argument("invalid polygon");
    }
    auto same_vertex_num = std::bind(is_equal_vertex_num, vertex_num, std::placeholders::_1);
    out << std::count_if(polygons.cbegin(), polygons.cend(), same_vertex_num);
  }
}
