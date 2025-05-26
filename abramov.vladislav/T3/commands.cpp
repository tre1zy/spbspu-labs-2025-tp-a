#include "commands.hpp"
#include <numeric>
#include <algorithm>
#include "geom.hpp"

void abramov::getCommands(std::map< std::string, std::function< void() > > &commands, std::vector< Polygon > &polygons)
{
  commands["AREA"] = std::bind(area, std::cref(polygons), std::ref(std::cout), std::ref(std::cin));
  commands["MAX"] = std::bind(max, std::cref(polygons), std::ref(std::cout), std::ref(std::cin));
  commands["MIN"] = std::bind(min, std::cref(polygons), std::ref(std::cout), std::ref(std::cin));
  commands["COUNT"] = std::bind(count, std::cref(polygons), std::ref(std::cout), std::ref(std::cin));
  commands["RMECHO"] = std::bind(rmecho, std::ref(polygons), std::ref(std::cout), std::ref(std::cin));
}

void abramov::area(const std::vector< Polygon > &polygons, std::ostream &out, std::istream &in)
{
  using namespace std::placeholders;

  std::string subcommand;
  in >> subcommand;
  double res = 0;
  if (subcommand == "EVEN")
  {
    res = std::accumulate(polygons.begin(), polygons.end(), 0.0, areaEven);
  }
  else if (subcommand == "ODD")
  {
    res = std::accumulate(polygons.begin(), polygons.end(), 0.0, areaOdd);
  }
  else if (subcommand == "MEAN")
  {
    res = areaMean(polygons);
  }
  else
  {
    size_t vert = 0;
    if (!(in >> vert))
    {
      throw std::logic_error("Unknown command\n");
    }
    if (vert < 3)
    {
      throw std::logic_error("Too less vertexes\n");
    }
    auto f = std::bind(areaVertexes, _1, _2, vert);
    res = std::accumulate(polygons.begin(), polygons.end(), 0.0, f);
  }
  out << res;
}

void abramov::max(const std::vector< Polygon> &polygons, std::ostream &out, std::istream &in)
{
  if (polygons.size() < 1)
  {
    throw std::logic_error("Can not find max\n");
  }
  std::string subcommand;
  in >> subcommand;
  if (subcommand == "AREA")
  {
    auto p = std::max_element(polygons.begin(), polygons.end(), maxArea);
    out << getArea(*p);
  }
  else if (subcommand == "VERTEXES")
  {
    auto p = std::max_element(polygons.begin(), polygons.end(), maxVertexes);
    out << (*p).points.size();
  }
  else
  {
    throw std::logic_error("Unknown command\n");
  }
}

void abramov::min(const std::vector< Polygon > &polygons, std::ostream &out, std::istream &in)
{
  if (polygons.size() < 1)
  {
    throw std::logic_error("Can not find min\n");
  }
  std::string subcommand;
  in >> subcommand;
  if (subcommand == "AREA")
  {
    auto p = std::min_element(polygons.begin(), polygons.end(), maxArea);
    out << getArea(*p);
  }
  else if (subcommand == "VERTEXES")
  {
    auto p = std::min_element(polygons.begin(), polygons.end(), maxVertexes);
    out << (*p).points.size();
  }
  else
  {
    throw std::logic_error("Unknown command\n");
  }
}

void abramov::count(const std::vector< Polygon > &polygons, std::ostream &out, std::istream &in)
{
  using namespace std::placeholders;

  std::string subcommand;
  in >> subcommand;
  size_t res = 0;
  if (subcommand == "EVEN")
  {
    res = countEven(polygons);
  }
  else if (subcommand == "ODD")
  {
    res = countOdd(polygons);
  }
  else
  {
    size_t vert = 0;
    if (!(in >> vert))
    {
      throw std::logic_error("Unknown command\n");
    }
    if (vert < 3)
    {
      throw std::logic_error("Too less vertexes\n");
    }
    auto f = std::bind(countVertexes, _1, _2, vert);
    res = std::accumulate(polygons.begin(), polygons.end(), 0, f);
  }
  out << res;
}

void abramov::rmecho(std::vector< Polygon > &polygons, std::ostream &out, std::istream &in)
{
  using namespace std::placeholders;

  Polygon example;
  in >> example;
  auto f = std::bind(isPolygonsEqualToExample, _1, _2, example);
  auto del = std::unique(polygons.begin(), polygons.end(), f);
  size_t diff = polygons.size();
  polygons.erase(del, polygons.end());
  diff -= polygons.size();
  out << diff;
}
