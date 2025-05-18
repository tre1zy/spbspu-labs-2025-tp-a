#include <iostream>
#include <fstream>
#include <limits>
#include <iterator>
#include <algorithm>
#include <functional>
#include <string>
#include <map>
#include "shapes.hpp"
#include "commands.hpp"

int main()
{
  using namespace tkach;
  using istreamIT = std::istream_iterator< Polygon >;
  std::fstream in("/home/danil/spbspu-labs-2025-tp-a/tkach.danil/T3/input.txt");
  if (!in.is_open())
  {
    std::cerr << "File is not open\n";
    return 1;
  }
  std::vector< Polygon > data;
  while (!in.eof())
  {
    std::copy(istreamIT{in}, istreamIT{}, std::back_inserter(data));
    if (in.fail())
    {
      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(printArea, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  std::string command;
  while (!(std::cin >> command).eof()) {
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
