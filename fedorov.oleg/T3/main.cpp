#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <limits>

#include "polygon.hpp"
#include "commands.hpp"
#include "functional.hpp"

int main(int argc, char *argv[])
{
  using namespace fedorov;
  using namespace std::placeholders;

  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::vector< Polygon > polygons;
  try
  {
    std::ifstream file(argv[1]);
    if (!file)
    {
      throw std::runtime_error("Failed to open file");
    }
    readPolygons(file, polygons);
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << '\n';
    return 2;
  }

  std::map< std::string, std::function< void(std::istream &, std::ostream &) > > commands;
  commands["AREA"] = std::bind(areaCommand, _1, _2, std::cref(polygons));
  commands["LESSAREA"] = std::bind(lessAreaCommand, _1, _2, std::cref(polygons));
  commands["MAX"] = std::bind(maxCommand, _1, _2, std::cref(polygons));
  commands["MIN"] = std::bind(minCommand, _1, _2, std::cref(polygons));
  commands["ECHO"] = std::bind(echoCommand, _1, _2, std::ref(polygons));
  commands["COUNT"] = std::bind(countCommand, _1, _2, std::cref(polygons));

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      auto it = commands.find(command);
      if (it != commands.end())
      {
        it->second(std::cin, std::cout);
      }
      else
      {
        throw std::invalid_argument("Unknown command");
      }
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
