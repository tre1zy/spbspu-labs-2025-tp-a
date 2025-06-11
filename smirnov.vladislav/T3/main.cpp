#include "geometry.hpp"
#include "commands.hpp"
#include <iostream>
#include <fstream>
#include <iterator>
#include <limits>
#include <map>
#include <functional>
#include <algorithm>

int main(int argc, char* argv[])
{
  using Polygon = geom::Polygon;
  using inputIt = std::istream_iterator< Polygon >;

  if (argc != 2)
  {
    std::cerr << "No file\n";
    return 1;
  }

  std::ifstream inFile(argv[1]);
  std::vector< geom::Polygon > polyList;

  while (!inFile.eof())
  {
    std::copy(inputIt(inFile), inputIt(), std::back_inserter(polyList));
    if (!inFile)
    {
      inFile.clear();
      inFile.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::map<std::string, std::function<void()>> commandMap;
  commandMap["AREA"] = std::bind(smirnov::printAreaSum, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["MAX"] = std::bind(smirnov::printMaxValueOf, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["MIN"] = std::bind(smirnov::printMinValueOf, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["COUNT"] = std::bind(smirnov::printCountOf, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["LESSAREA"] = std::bind(smirnov::printLessAreaCnt, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["INTERSECTIONS"] = std::bind(smirnov::printIntersectionsCnt, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));

  std::string cmd;
  while (!(std::cin >> cmd).eof())
  {
    try
    {
      commandMap.at(cmd)();
      std::cout << '\n';
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
