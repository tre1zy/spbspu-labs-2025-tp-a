#include <iterator>
#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
#include "io_polygon.hpp"

namespace
{
  int checkArguments(const int argc);

  int checkArguments(const int argc)
  {
    if (argc == 2)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
}

int main(int argc, const char * const * argv)
{
  using namespace petrov;
  using polygon_istream_it = std::istream_iterator< Polygon >;
  if (!checkArguments(argc))
  {
    std::cerr << "ERROR: Too many arguments";
    std::cerr << "\n";
    return 1;
  }
  std::vector< Polygon > polygons;
  std::ifstream input(argv[1]);
  while (!input.eof())
  {
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    std::copy(polygon_istream_it(input), polygon_istream_it(), std::back_inserter(polygons));
  }
  for (size_t i = 0; i < polygons.size(); i++)
  {
    std::cout << polygons[i];
    std::cout << "\n";
  }
}
