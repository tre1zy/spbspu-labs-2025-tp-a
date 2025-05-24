#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <functional>
#include "polygon.hpp"
#include "commands.hpp"

using std::placeholders::_1;
using std::placeholders::_2;

int main(int argc, char *argv[])
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
  std::string line;

  // I know this is kind of memory inefficient
  // but I didn't find a solution using std::istream_iterator...
  // it just doesn't want to work properly with wrong Polygon entries from the fstream
  while (std::getline(inputFile, line))
  {
    std::istringstream iss(line);
    puzikov::Polygon poly;
    if (iss >> poly)
    {
      polygons.push_back(poly);
    }
  }

  inputFile.clear();
  inputFile.close();

  std::copy(polygons.begin(), polygons.end(), output_it_t {std::cout, "\n"});

  std::string cmd;
  std::map< std::string, std::function< void(std::istream &, std::ostream &) > > commands {
      {"AREA", std::bind(puzikov::areaCommand, _1, _2, std::ref(polygons))},
      {"MAX", std::bind(puzikov::maxCommand, _1, _2, std::ref(polygons))},
      {"MIN", std::bind(puzikov::minCommand, _1, _2, std::ref(polygons))},
      {"COUNT", std::bind(puzikov::countCommand, _1, _2, std::ref(polygons))},
      {"RMECHO", std::bind(puzikov::rmEchoCommand, _1, _2, std::ref(polygons))},
      {"SAME", std::bind(puzikov::sameCommand, _1, _2, std::ref(polygons))}};

  while (!std::cin.eof())
  {
    std::cin >> cmd;
    try
    {
      commands.at(cmd)(std::cin, std::cout);
    }
    catch (...)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
