#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <functional>
#include <map>
#include "polygon.hpp"
#include "commandHandling.hpp"
#include "exceptions.hpp"

int main(int argc, char** argv)
{
  using namespace holodilov;

  if (argc != 2)
  {
    std::cout << "Error: enter filename as console argument:\n";
    std::cout << argv[0] << " <filename>\n";
    return 1;
  }

  std::ifstream fis(argv[1]);
  if (!fis.is_open())
  {
    std::cout << "Error: cannot open this file.\n";
    return 1;
  }

  using istreamIter = std::istream_iterator< Polygon >;
  std::vector< Polygon > vecPolygons;
  while (!fis.eof())
  {
    std::copy(istreamIter(fis), istreamIter(), std::back_inserter(vecPolygons));
    if (!fis)
    {
      fis.clear(fis.rdstate() ^ std::ios::failbit);
      fis.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  fis.close();

  std::map< std::string, std::function< void() > > mapCommands;
  mapCommands["ECHO"] = std::bind(commands::echo, std::ref(std::cin), std::ref(std::cout), std::ref(vecPolygons));
  mapCommands["LESSAREA"] = std::bind(commands::lessArea, std::ref(std::cin), std::ref(std::cout), std::cref(vecPolygons));
  while (!std::cin.eof())
  {
    std::string commandName;
    std::cin >> commandName;
    try
    {
      mapCommands.at(commandName)();
    }
    catch (const std::out_of_range& e)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      continue;
    }
    catch (const InvalidCommandException& e)
    {
      std::cout << e.what();
    }

    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}