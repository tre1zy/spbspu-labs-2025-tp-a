#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <fstream>
#include "polygon.hpp"
#include "command_handler.hpp"

int main(int argc, char **argv)
{
  using puzikov::Polygon;
  using output_it_t = std::ostream_iterator< Polygon >;

  if (argc != 2)
  {
    std::cerr << "You need to provide a filename 👺\n";
    return 1;
  }

  std::ifstream inputFile {argv[1]};
  if (!inputFile)
  {
    std::cerr << "Couldn't open the file 😢\n";
    return 1;
  }

  std::vector< puzikov::Polygon > polygons;
  puzikov::readPolygons(inputFile, polygons);
  inputFile.close();

  std::copy(polygons.begin(), polygons.end(), output_it_t {std::cout, "\n"});
  puzikov::CommandHandler cmdHandler(polygons);

  cmdHandler.readCommands(std::cin, std::cout);
}
