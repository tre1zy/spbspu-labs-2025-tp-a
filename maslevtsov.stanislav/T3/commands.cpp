#include "commands.hpp"
#include <algorithm>
#include <functional>
#include <numeric>
#include <io_fmt_guard.hpp>

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
    return -lhs.y * rhs.x;
  }

  double get_polygon_area(const maslevtsov::Polygon& polygon)
  {
    std::vector< maslevtsov::Point > second_lace;
    std::copy(++polygon.points.cbegin(), polygon.points.cend(), std::back_inserter(second_lace));
    second_lace.push_back(*polygon.points.cbegin());
    std::vector< double > to_accumulate(polygon.points.size());
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
    return lhs.points.size() < rhs.points.size();
  }

  bool is_same(const maslevtsov::Polygon& lhs, const maslevtsov::Polygon& rhs)
  {
    if (lhs.points.size() != rhs.points.size()) {
      return false;
    }
    return std::mismatch(lhs.points.cbegin(), lhs.points.cend(), rhs.points.cbegin()).first == lhs.points.cend();
  }

  struct WithSamesInserter
  {
    std::vector< maslevtsov::Polygon >& with_echos;

    int operator()(maslevtsov::Polygon& polygon, const maslevtsov::Polygon& to_compare)
    {
      auto same_with_arg = std::bind(is_same, to_compare, std::placeholders::_1);
      with_echos.push_back(polygon);
      if (same_with_arg(polygon)) {
        with_echos.push_back(polygon);
      }
      return 0;
    }
  };
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

void maslevtsov::echo(std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  Polygon polygon;
  if (!(in >> polygon)) {
    throw std::invalid_argument("invalid polygon");
  }
  auto same_with_arg = std::bind(is_same, polygon, std::placeholders::_1);
  std::size_t additional_size = std::count_if(polygons.begin(), polygons.end(), same_with_arg);
  std::vector< Polygon > with_echoes(polygons.size() + additional_size);
  std::vector< int > dump(polygons.size() + additional_size);
  auto inserter = std::bind(WithSamesInserter{with_echoes}, std::placeholders::_1, polygon);
  std::transform(polygons.begin(), polygons.end(), dump.begin(), inserter);
  polygons = with_echoes;
  out << additional_size;
}

void maslevtsov::remove_echo(std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  Polygon polygon;
  if (!(in >> polygon)) {
    throw std::invalid_argument("invalid polygon");
  }
  auto same_with_arg = std::bind(is_same, polygon, std::placeholders::_1);
  auto same_first_with_arg = std::bind(same_with_arg, std::placeholders::_1);
  auto same_second_with_arg = std::bind(same_with_arg, std::placeholders::_2);
  auto same_both_to_arg = std::bind(std::logical_and< bool >{}, same_first_with_arg, same_second_with_arg);
  auto first_to_erase = std::unique(polygons.begin(), polygons.end(), same_both_to_arg);
  out << std::distance(first_to_erase, polygons.end());
  polygons.erase(first_to_erase, polygons.end());
}
