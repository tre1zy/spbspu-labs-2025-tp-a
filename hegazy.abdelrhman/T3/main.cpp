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
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::vector< Polygon > polyList;
  std::ifstream inFile(argv[1]);

  if (!inFile)
  {
    std::cerr << "Error: Cannot open file " << argv[1] << "\n";
    return 1;
  }

  while (inFile)
  {
    Polygon poly;
    if (inFile >> poly)
    {
      polyList.push_back(poly);
    }
    else if (!inFile.eof())
    {
      inFile.clear();
      inFile.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::map< std::string, std::function< void() > > commandMap;
  commandMap["AREA"] = std::bind(bob::printAreaSum, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["MAX"] = std::bind(bob::printMaxValueOf, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["MIN"] = std::bind(bob::printMinValueOf, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["COUNT"] = std::bind(bob::printCountOf, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["LESSAREA"] = std::bind(bob::printLessAreaCnt, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["INTERSECTIONS"] = std::bind(bob::printIntersectionsCnt, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["ECHO"] = []() { std::cout << "<INVALID COMMAND>"; };
  commandMap["INFRAME"] = []() { std::cout << "<INVALID COMMAND>"; };
  commandMap["MAXSEQ"] = []() { std::cout << "<INVALID COMMAND>"; };
  commandMap["PERMS"] = []() { std::cout << "<INVALID COMMAND>"; };
  commandMap["SAME"] = []() { std::cout << "<INVALID COMMAND>"; };

  std::string line;
  while (std::cin >> line)
  {
    try
    {
      auto it = commandMap.find(line);
      if (it != commandMap.end())
      {
        it->second();
        std::cout << "\n";
      }
      else
      {
        throw std::invalid_argument("Unknown command");
      }
    }
    catch (const std::exception& e)
    {
      std::cout << "<INVALID COMMAND>\n";
      if (std::cin.fail())
      {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
  }
  
  return 0;
}