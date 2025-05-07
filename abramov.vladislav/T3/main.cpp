#include <vector>
#include <iterator>
#include <fstream>
#include <limits>
#include "geom.hpp"

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    std::cerr << "wrong filename\n";
    return 1;
  }
  std::ifstream input(argv[1]);
  using namespace abramov;
  std::vector< Polygon > polygons;
  while (!input.eof())
  {
    try
    {
      std::copy(std::istream_iterator< Polygon >(input), std::istream_iterator< Polygon >(), std::back_inserter(polygons));
    }
    catch (std::logic_error &)
    {
      input.clear(input.rdstate() ^ std::ios::failbit);
      input.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
