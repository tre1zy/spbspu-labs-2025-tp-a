#include <iostream>
#include <vector>
#include <iterator>
#include <limits>
#include <map>
#include <functional>
#include "cmds.hpp"
#include "GeometricalTypes.hpp"

int main()
{
  using shapkov::Polygon;
  std::vector< Polygon > data(4);
  std::cin >> data[0];
  std::cin >> data[1];
  std::cin >> data[2];
  std::cin >> data[3];

  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(shapkov::area, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["MAX"] = std::bind(shapkov::max, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["MIN"] = std::bind(shapkov::min, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["COUNT"] = std::bind(shapkov::count, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["RECTS"] = std::bind(shapkov::rects, std::ref(std::cout), std::cref(data));
  cmds["SAME"] = std::bind(shapkov::same, std::ref(std::cin), std::ref(std::cout), std::cref(data));

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
  return 0;
}
