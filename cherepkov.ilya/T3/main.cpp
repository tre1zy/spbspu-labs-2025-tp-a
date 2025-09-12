#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <fstream>
#include "shapes.hpp"

int main(int argc, char* argv[])
{
  using namespace cherepkov;

  if (argc != 2)
  {
    std::cerr << "ERROR: Incorrect number of arguments\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "ERROR: the file is unreadable\n";
    return 1;
  }

  std::vector< Polygon > polygons;

  using input_iter = std::istream_iterator< Polygon >;
  while (!file.eof())
  {
    std::copy(input_iter{in}, input_iter{}, std::back_inserter(polygons));
    if (!file)
    {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

}
