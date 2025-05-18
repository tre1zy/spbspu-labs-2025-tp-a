#include <iostream>
#include <fstream>
#include <limits>
#include <iterator>
#include <algorithm>
#include <functional>
#include <string>
#include <map>
#include "shapes.hpp"

int main(const int argc, const char* const * const argv)
{
  using namespace tkach;
  using istreamIT = std::istream_iterator< Polygon >;
  using ostreamIT = std::ostream_iterator< Polygon >;
  if (argc != 2)
  {
    std::cerr << "Error: incorrect input\n";
    return 1;
  }
  std::fstream in(argv[1]);
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
