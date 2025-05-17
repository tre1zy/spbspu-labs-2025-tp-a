#include <functional>
#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include "geom.hpp"

namespace aleksandrov
{
  void area(std::ostream& out)
  {
    out << "area\n";
  }
  void max(std::ostream& out)
  {
    out << "max\n";
  }
  void min(std::ostream& out)
  {
    out << "min\n";
  }
  void count(std::ostream& out)
  {
    out << "count\n";
  }
}

int main()
{
  using namespace aleksandrov;
  std::list< Point > points;

  std::map< std::string, std::function< void(void) > > commands;
  commands["AREA"] = std::bind(area, std::ref(std::cout));
  commands["MAX"] = std::bind(max, std::ref(std::cout));
  commands["MIN"] = std::bind(min, std::ref(std::cout));
  commands["COUNT"] = std::bind(count, std::ref(std::cout));
  
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      commands.at(command)();
    }
    catch (...)
    {
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}

