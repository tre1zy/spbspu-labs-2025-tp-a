#include <fstream>
#include <iterator>
#include <string>
#include <limits>
#include "polygon.hpp"

int main(int argc, char * argv[])
{
  using namespace smirnov;
  if (argc != 2)
  {
    std::cerr << "Error: filename required\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "Error: can't open file\n";
    return 2;
  }
  std::vector< Polygon > polygons;
  while (!file.eof())
  {
    using input_it = std::istream_iterator< Polygon >;
    std::copy(input_it{file}, input_it{}, std::back_inserter(polygons));
    if (file.fail() && !file.eof())
    {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::cout << "Прочитано фигур: " << polygons.size() << "\n";
  return 0;
}

