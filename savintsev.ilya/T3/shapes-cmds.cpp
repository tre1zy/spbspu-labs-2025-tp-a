#include "shapes-cmds.h"
#include <string>
#include <algorithm>
#include <numeric>
#include <cmath>

namespace
{
  bool is_polygon_even(const savintsev::Polygon & a)
  {
    return !(a.points.size() % 2);
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
  std::string subcommand;
  in >> subcommand;
  if (subcommand == "EVEN")
  {
    data.size();
    //double area = 0;
    //std::accumulate(data.begin(), data.end(), area);
  }
  else if (subcommand == "ODD")
  {

  }
  out << "lox\n";
}

void savintsev::count(std::istream & in, std::ostream & out, const std::vector< Polygon > & data)
{
  size_t num = 0;
  in >> num;
  if (in)
  {
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
    out << data.size() - std::count_if(data.begin(), data.end(), is_polygon_even);
  }
  else
  {
    throw std::runtime_error("count: invalid arg");
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

double savintsev::calc_polygon_area(Polygon a)
{
  size_t n = a.points.size();
  if (n < 3)
  {
    return 0.0;
  }

  double area = 0.0;
  for (size_t i = 0; i < n; ++i)
  {
    size_t j = (i + 1) % n;
    area += (a.points[i].x * a.points[j].y) - (a.points[j].x * a.points[i].y);
  }
  return 0.5 * abs(area);
}
