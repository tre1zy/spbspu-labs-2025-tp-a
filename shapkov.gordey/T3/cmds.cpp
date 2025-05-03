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
    throw std::logic_error("points empty");
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
  out << area;
}
/*void shapkov::max(std::istream& in, std::ostream& out, const VecOfPolygons& src)
{

}
void shapkov::min(std::istream& in, std::ostream& out, const VecOfPolygons& src)
{

}
void shapkov::count(std::istream& in, std::ostream& out, const VecOfPolygons& src)
{

}*/
