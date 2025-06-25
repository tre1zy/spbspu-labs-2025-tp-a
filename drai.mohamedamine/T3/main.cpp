#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include "polygon.hpp"

namespace amine
{
  struct PolygonFilter
  {
    bool operator()(const Polygon& p) const
    {
      return p.points.size() >= 3;
    }
  };
}

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

  std::copy_if(
    std::istream_iterator< amine::Polygon >(infile),
    std::istream_iterator< amine::Polygon >(),
    std::back_inserter(polygons),
    amine::PolygonFilter{}
  );

  amine::process_commands(polygons);
  return 0;
}
