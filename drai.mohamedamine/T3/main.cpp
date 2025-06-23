#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include "polygon.hpp"

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "Error: missing filename argument\n";
    return 1;
  }
  std::ifstream infile(argv[1]);
  if (!infile)
  {
    std::cerr << "Error: could not open file\n";
    return 1;
  }

  std::vector< amine::Polygon > polygons;
  std::vector< amine::Line > lines;

  std::copy(std::istream_iterator< amine::Line >(infile), std::istream_iterator< amine::Line >(),
            std::back_inserter(lines));

  std::transform(lines.begin(), lines.end(), std::back_inserter(polygons), [](const amine::Line& line) {
    amine::Polygon poly;
    if (!line.content.empty() && amine::parse_polygon(line.content, poly) && poly.points.size() >= 3)
    {
      return poly;
    }
    return amine::Polygon{};
  });

  polygons.erase(
    std::remove_if(polygons.begin(), polygons.end(), [](const amine::Polygon& p) { return p.points.empty(); }),
    polygons.end());

  amine::process_commands(polygons);
  return 0;
}
