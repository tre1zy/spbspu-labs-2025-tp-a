#include "commands.hpp"
#include <algorithm>
#include <functional>
#include <numeric>

namespace {

  bool is_even_vertex_num(const maslevtsov::Polygon& polygon)
  {
    return polygon.points.size() % 2 == 0;
  }

  bool is_odd_vertex_num(const maslevtsov::Polygon& polygon)
  {
    return !is_even_vertex_num(polygon);
  }

  bool is_equal_vertex_num(std::size_t vertex_num, const maslevtsov::Polygon& polygon)
  {
    return vertex_num == polygon.points.size();
  }

  double add_main_diagonal(const maslevtsov::Point& lhs, const maslevtsov::Point& rhs)
  {
    return lhs.x * rhs.y;
  }

  double subtract_sub_diagonal(const maslevtsov::Point& lhs, const maslevtsov::Point& rhs)
  {
    return -(lhs.y * rhs.x);
  }

  double get_polygon_area(const maslevtsov::Polygon& polygon)
  {
    std::vector< maslevtsov::Point > second_lace;
    std::copy(++polygon.points.cbegin(), polygon.points.cend(), std::back_inserter(second_lace));
    second_lace.push_back(*polygon.points.cbegin());
    std::vector< double > to_accumulate;
    to_accumulate.reserve(polygon.points.size());
    auto begin_it = polygon.points.cbegin();
    auto end_it = polygon.points.cend();
    double area = 0.0;
    std::transform(begin_it, end_it, second_lace.cbegin(), to_accumulate.begin(), add_main_diagonal);
    area += std::accumulate(to_accumulate.cbegin(), to_accumulate.cend(), 0.0);
    std::transform(begin_it, end_it, second_lace.cbegin(), to_accumulate.begin(), subtract_sub_diagonal);
    area += std::accumulate(to_accumulate.cbegin(), to_accumulate.cend(), 0.0);
    area *= 0.5;
    area = std::abs(area);
    return area;
  }

  bool compare_vertex_num_less(const maslevtsov::Polygon& lhs, const maslevtsov::Polygon& rhs)
  {
    return lhs.points.size() < lhs.points.size();
  }
}

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
    return;
  } else {
    std::size_t vertex_num = std::stoull(subcommand);
    if (vertex_num < 3) {
      throw std::invalid_argument("invalid polygon");
    }
    auto vertex_predicate = std::bind(is_equal_vertex_num, vertex_num, std::placeholders::_1);
    std::copy_if(polygons.cbegin(), polygons.cend(), std::back_inserter(filtered), vertex_predicate);
  }
  std::vector< double > to_accumulate;
  std::transform(filtered.cbegin(), filtered.cend(), std::back_inserter(to_accumulate), get_polygon_area);
  double result = std::accumulate(to_accumulate.cbegin(), to_accumulate.cend(), 0.0);
  if (subcommand == "MEAN") {
    result /= filtered.size();
  }
  out << result << '\n';
}

void maslevtsov::get_max(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::string subcommand;
  in >> subcommand;
  if (subcommand == "AREA") {
    std::vector< double > areas;
    std::transform(polygons.cbegin(), polygons.cend(), std::back_inserter(areas), get_polygon_area);
    out << *std::max_element(areas.cbegin(), areas.cend()) << '\n';
  } else if (subcommand == "VERTEXES") {
    out << std::max_element(polygons.cbegin(), polygons.cend(), compare_vertex_num_less)->points.size() << '\n';
  } else {
    throw std::invalid_argument("invalid subcommand");
  }
}

void maslevtsov::get_min(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::string subcommand;
  in >> subcommand;
  if (subcommand == "AREA") {
    std::vector< double > areas;
    std::transform(polygons.cbegin(), polygons.cend(), std::back_inserter(areas), get_polygon_area);
    out << *std::min_element(areas.cbegin(), areas.cend()) << '\n';
  } else if (subcommand == "VERTEXES") {
    out << std::min_element(polygons.cbegin(), polygons.cend(), compare_vertex_num_less)->points.size() << '\n';
  } else {
    throw std::invalid_argument("invalid subcommand");
  }
}

void maslevtsov::count_vertexes(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::size_t temp = 0;
  temp = polygons.empty();
  ++temp;
  std::string subcommand;
  in >> subcommand;
  out << "COUNT " << subcommand << '\n';
}

void maslevtsov::echo(std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::size_t temp = 0;
  temp = polygons.empty();
  ++temp;
  std::string subcommand;
  in >> subcommand;
  out << "ECHO " << subcommand << '\n';
}

void maslevtsov::remove_echo(std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::size_t temp = 0;
  temp = polygons.empty();
  ++temp;
  std::string subcommand;
  in >> subcommand;
  out << "RMECHO " << subcommand << '\n';
}
