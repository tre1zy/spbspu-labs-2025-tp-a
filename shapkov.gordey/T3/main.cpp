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
  for (size_t i = 0; i < 4; i++)
  {
    std::cout << shapkov::getArea(data[i]) << '\n';
  }
  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(shapkov::area, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  //cmds["max"] = std::bind(kas::max, std::cref(points), std::ref(std::cin), std::ref(std::cout));
  //cmds["add"] = std::bind(kas::add, std::ref(points), std::ref(std::cin));

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
