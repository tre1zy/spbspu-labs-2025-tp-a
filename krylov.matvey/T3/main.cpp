#include <map>
#include <limits>
#include <vector>
#include <fstream>
#include <iterator>
#include <functional>
#include "geometry.hpp"

int main(int argc, char **argv)
{
  using namespace krylov;

  if (argc != 2)
  {
    std::cerr << "Wrong parameters!\n";
    return 1;
  }
  std::ifstream input(argv[1]);
  if (!input)
  {
    std::cerr << "Wrong parameters!\n";
    return 1;
  }
  std::vector< Polygon > polygons;
  while (!input.eof())
  {
    std::copy(std::istream_iterator< Polygon >(input), std::istream_iterator< Polygon >(), std::back_inserter(polygons));
    if (!input)
    {
      input.clear();
      input.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
