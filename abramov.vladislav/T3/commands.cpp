#include "commands.hpp"
#include <numeric>
#include "geom.hpp"

void abramov::getCommands(std::map< std::string, std::function< void() > > &commands, std::vector< Polygon > &polygons)
{
  commands["AREA"] = std::bind(abramov::area, std::cref(polygons), std::ref(std::cout), std::ref(std::cin));
  commands["MAX"] = std::bind(abramov::max, std::cref(polygons), std::ref(std::cout), std::ref(std::cin));
  commands["MIN"] = std::bind(abramov::min, std::cref(polygons), std::ref(std::cout), std::ref(std::cin));
  commands["COUNT"] = std::bind(abramov::count, std::cref(polygons), std::ref(std::cout), std::ref(std::cin));
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
  size_t k = 0;
  in >> k;
  out << k;
  polygons[0];
  return;
}

void abramov::min(const std::vector< Polygon > &polygons, std::ostream &out, std::istream &in)
{
  size_t k = 0;
  in >> k;
  out << k;
  polygons[0];
  return;
}

void abramov::count(const std::vector< Polygon > &polygons, std::ostream &out, std::istream &in)
{
  size_t k = 0;
  in >> k;
  out << k;
  polygons[0];
  return;
}
