#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <limits>
#include <iterator>
#include <algorithm>
#include <functional>
#include "shapes.hpp"

int main(int argc, char * argv[])
{
  using mozhegova::Polygon;
  using istreamIter = std::istream_iterator< Polygon >;
  if (argc != 2)
  {
    std::cout << "incorrect input!!!\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cout << "file is not open!!!\n";
    return 1;
  }
  std::vector< Polygon > polygons;
  while (!file.eof())
  {
    std::copy(istreamIter{file}, istreamIter{}, std::back_inserter(polygons));
    if (file.fail())
    {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::map< std::string, std::function< void() > > cmds;
  // cmds["show"] = std::bind(kas::show, std::cref(points), std::ref(std::cout));

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
    }
    catch (...)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
