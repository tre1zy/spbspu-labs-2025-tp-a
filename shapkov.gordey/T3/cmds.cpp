#include "cmds.hpp"
#include <string>
#include <functional>
#include <algorithm>
#include <numeric>
#include <iterator>
#include "functors.hpp"

void shapkov::area(std::istream& in, std::ostream& out, const VecOfPolygons& src)
{
  if (src.empty())
  {
    throw std::logic_error("no polygons");
  }
  std::string subcommand;
  in >> subcommand;
  double area = 0.0;
  if (subcommand == "EVEN")
  {
    VecOfPolygons evenPolygons;
    std::copy_if(src.begin(), src.end(), std::back_inserter(evenPolygons), isEven);
    std::vector< double > areas(evenPolygons.size());
    std::transform(evenPolygons.begin(), evenPolygons.end(), areas.begin(), getArea);
    area = std::accumulate(areas.begin(), areas.end(), 0.0);
  }
  else if (subcommand == "ODD")
  {
    VecOfPolygons oddPolygons;
    std::copy_if(src.begin(), src.end(), std::back_inserter(oddPolygons), isOdd);
    std::vector< double > areas(oddPolygons.size());
    std::transform(oddPolygons.begin(), oddPolygons.end(), areas.begin(), getArea);
    area = std::accumulate(areas.begin(), areas.end(), 0.0);
  }
  else if (subcommand == "MEAN")
  {
    std::vector< double > areas(src.size());
    std::transform(src.begin(), src.end(), areas.begin(), getArea);
    area = std::accumulate(areas.begin(), areas.end(), 0.0) / src.size();
  }
  else
  {
    size_t vertexes = std::stoi(subcommand);
    VecOfPolygons Polygons;
    std::copy_if(src.begin(), src.end(), std::back_inserter(Polygons), HasSize{vertexes});
    std::vector< double > areas(Polygons.size());
    std::transform(Polygons.begin(), Polygons.end(), areas.begin(), getArea);
    area = std::accumulate(areas.begin(), areas.end(), 0.0);
  }
  out << area << '\n';
}
void shapkov::max(std::istream& in, std::ostream& out, const VecOfPolygons& src)
{
  if (src.empty())
  {
    throw std::logic_error("no polygons");
  }
  std::string subcommand;
  in >> subcommand;
  if (subcommand == "AREA")
  {
    std::vector< double > areas(src.size());
    std::transform(src.begin(), src.end(), areas.begin(), getArea);
    out << (*std::max_element(areas.begin(), areas.end())) << '\n';
  }
  else if (subcommand == "VERTEXES")
  {
    out << (std::max_element(src.begin(), src.end(), compareByVertexes)->points.size()) << '\n';
  }
  else
  {
    throw std::logic_error("unknown subcommand");
  }
}
void shapkov::min(std::istream& in, std::ostream& out, const VecOfPolygons& src)
{
  if (src.empty())
  {
    throw std::logic_error("no polygons");
  }
  std::string subcommand;
  in >> subcommand;
  if (subcommand == "AREA")
  {
    std::vector< double > areas(src.size());
    std::transform(src.begin(), src.end(), areas.begin(), getArea);
    out << (*std::min_element(areas.begin(), areas.end())) << '\n';
  }
  else if (subcommand == "VERTEXES")
  {
    out << (std::min_element(src.begin(), src.end(), compareByVertexes)->points.size()) << '\n';
  }
  else
  {
    throw std::logic_error("unknown subcommand");
  }
}
void shapkov::count(std::istream& in, std::ostream& out, const VecOfPolygons& src)
{
  if (src.empty())
  {
    throw std::logic_error("no polygons");
  }
  std::string subcommand;
  in >> subcommand;
  if (subcommand == "EVEN")
  {
    out << std::count_if(src.begin(), src.end(), isEven) << '\n';
  }
  else if (subcommand == "ODD")
  {
    out << std::count_if(src.begin(), src.end(), isOdd) << '\n';
  }
  else
  {
    size_t vertexes = std::stoi(subcommand);
    out << std::count_if(src.begin(), src.end(), HasSize{vertexes}) << '\n';
  }
}
