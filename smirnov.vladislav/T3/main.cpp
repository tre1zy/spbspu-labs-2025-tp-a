#include <iostream>
#include <fstream>
#include <iterator>
#include <limits>
#include <map>
#include <functional>
#include <algorithm>
#include <string>
#include <sstream>
#include "geometry.hpp"
#include "commands.hpp"

int main(int argc, char* argv[])
{
  using Polygon = geom::Polygon;
  using it = std::istream_iterator< Polygon >;

  if (argc != 2)
  {
    return 1;
  }

  std::vector< Polygon > polyList;
  std::ifstream inFile(argv[1]);
  while (!file.eof())
  {
    std::copy(it(inFile), it(), std::back_inserter(polyList));
    if (!file)
    {
      inFile.clear();
      inFile.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
}

  std::map< std::string, std::function< void(std::istream&) > > commandMap;
  commandMap["AREA"] = std::bind(smirnov::printAreaSum,
    std::placeholders::_1,
    std::cref(polyList),
    std::ref(std::cout));
  commandMap["MAX"] = std::bind(smirnov::printMaxValueOf,
    std::placeholders::_1,
    std::cref(polyList),
    std::ref(std::cout));
  commandMap["MIN"] = std::bind(smirnov::printMinValueOf,
    std::placeholders::_1,
    std::cref(polyList),
    std::ref(std::cout));
  commandMap["COUNT"] = std::bind(smirnov::printCountOf,
    std::placeholders::_1,
    std::cref(polyList),
    std::ref(std::cout));
  commandMap["LESSAREA"] = std::bind(smirnov::printLessAreaCnt,
    std::placeholders::_1,
    std::cref(polyList),
    std::ref(std::cout));
  commandMap["INTERSECTIONS"] = std::bind(smirnov::printIntersectionsCnt,
    std::placeholders::_1,
    std::cref(polyList),
    std::ref(std::cout));

  std::string line;
  while (std::cin >> line)
  {
    auto it = commandMap.find(line);
    if (it == commandMap.end())
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      continue;
    }
    try
    {
      it->second(std::cin);
      std::cout << '\n';
    }
    catch (const std::exception& e)
    {
      std::cout << "<INVALID COMMAND>\n";
      if (std::cin.fail())
      {
        std::cin.clear();
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
