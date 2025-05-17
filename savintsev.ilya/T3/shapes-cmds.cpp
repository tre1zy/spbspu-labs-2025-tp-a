#include "shapes-cmds.h"
#include <string>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <cmath>
#include <scope-guard.hpp>

namespace
{
  double sum_with_area(double sum, const savintsev::Polygon & a)
  {
    return savintsev::calc_polygon_area(a) + sum;
  }
  bool is_polygon_even(const savintsev::Polygon & a)
  {
    return !(a.points.size() % 2);
  }
  bool is_polygon_odd(const savintsev::Polygon & a)
  {
    return a.points.size() % 2;
  }
  bool compare_areas(const savintsev::Polygon & a, const savintsev::Polygon & b)
  {
    return calc_polygon_area(a) < calc_polygon_area(b);
  }
  bool compare_vert_amt(const savintsev::Polygon & a, const savintsev::Polygon & b)
  {
    return a.points.size() < b.points.size();
  }
  struct CheckPolygonVert
  {
    bool operator()(const savintsev::Polygon & a)
    {
      return a.points.size() == num;
    }
    size_t num;
  };
}

void savintsev::area(std::istream & in, std::ostream & out, const std::vector< Polygon > & data)
{
  size_t num = 0;
  ScopeGuard guard(out);
  out.precision(1);
  out << std::fixed;
  in >> num;
  if (in)
  {
    if (num <= 2)
    {
      throw std::runtime_error("area: invalid arg");
    }
    CheckPolygonVert is{num};
    std::vector< Polygon > temp;
    std::copy_if(data.begin(), data.end(), std::back_inserter(temp), is);
    out << std::accumulate(temp.begin(), temp.end(), 0.0, sum_with_area) << '\n';
    return;
  }
  in.clear();
  std::string subcommand;
  in >> subcommand;
  if (subcommand == "EVEN")
  {
    std::vector< Polygon > temp;
    std::copy_if(data.begin(), data.end(), std::back_inserter(temp), is_polygon_even);
    out << std::accumulate(temp.begin(), temp.end(), 0.0, sum_with_area);
  }
  else if (subcommand == "ODD")
  {
    std::vector< Polygon > temp;
    std::copy_if(data.begin(), data.end(), std::back_inserter(temp), is_polygon_odd);
    out << std::accumulate(temp.begin(), temp.end(), 0.0, sum_with_area);
  }
  else if (subcommand == "MEAN")
  {
    if (data.empty())
    {
      throw std::runtime_error("area: div by zero: no polygons");
    }
    out << std::accumulate(data.begin(), data.end(), 0.0, sum_with_area) / data.size();
  }
  else
  {
    throw std::runtime_error("area: invalid arg");
  }
  out << '\n';
}

void savintsev::count(std::istream & in, std::ostream & out, const std::vector< Polygon > & data)
{
  size_t num = 0;
  in >> num;
  if (in)
  {
    if (num <= 2)
    {
      throw std::runtime_error("count: invalid arg");
    }
    CheckPolygonVert is{num};
    out << std::count_if(data.begin(), data.end(), is) << '\n';
    return;
  }
  in.clear();
  std::string subcommand;
  in >> subcommand;
  if (subcommand == "EVEN")
  {
    out << std::count_if(data.begin(), data.end(), is_polygon_even);
  }
  else if (subcommand == "ODD")
  {
    out << std::count_if(data.begin(), data.end(), is_polygon_odd);
  }
  else
  {
    throw std::runtime_error("count: invalid arg");
  }
  out << '\n';
}

void savintsev::max(std::istream & in, std::ostream & out, const std::vector< Polygon > & data)
{
  std::string subcommand;
  in >> subcommand;
  if (data.empty())
  {
    throw std::runtime_error("max: no polygons for search");
  }
  if (subcommand == "AREA")
  {
    ScopeGuard guard(out);
    out.precision(1);
    out << std::fixed;
    out << calc_polygon_area(*(std::max_element(data.begin(), data.end(), compare_areas)));
  }
  else if (subcommand == "VERTEXES")
  {
    out << std::max_element(data.begin(), data.end(), compare_vert_amt)->points.size();
  }
  else
  {
    throw std::runtime_error("max: invalid arg");
  }
  out << '\n';
}

void savintsev::min(std::istream & in, std::ostream & out, const std::vector< Polygon > & data)
{
  std::string subcommand;
  in >> subcommand;
  if (data.empty())
  {
    throw std::runtime_error("max: no polygons for search");
  }
  if (subcommand == "AREA")
  {
    ScopeGuard guard(out);
    out.precision(1);
    out << std::fixed;
    out << calc_polygon_area(*(std::min_element(data.begin(), data.end(), compare_areas)));
  }
  else if (subcommand == "VERTEXES")
  {
    out << std::min_element(data.begin(), data.end(), compare_vert_amt)->points.size();
  }
  else
  {
    throw std::runtime_error("min: invalid arg");
  }
  out << '\n';
}

void savintsev::print(std::ostream & out, const std::vector< Polygon > & data)
{
  for (size_t i = 0; i < data.size(); ++i)
  {
    out << data[i] << '\n';
  }
}
