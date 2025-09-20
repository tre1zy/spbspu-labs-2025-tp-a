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

  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::vector< Polygon > polyList;
  std::ifstream inFile(argv[1]);
  if (!inFile)
  {
    std::cerr << "Cannot open file: " << argv[1] << "\n";
    return 1;
  }

  // Read polygons from file
  std::string line;
  while (std::getline(inFile, line))
  {
    if (line.empty()) continue;
    
    std::istringstream iss(line);
    Polygon poly;
    if (iss >> poly)
    {
      polyList.push_back(poly);
    }
  }

  // Create command map
  std::map< std::string, std::function< void() > > commandMap;
  commandMap["AREA"] = std::bind(bob::printAreaSum, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["MAX"] = std::bind(bob::printMaxValueOf, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["MIN"] = std::bind(bob::printMinValueOf, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["COUNT"] = std::bind(bob::printCountOf, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["LESSAREA"] = std::bind(bob::printLessAreaCnt, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["INTERSECTIONS"] = std::bind(bob::printIntersectionsCnt, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  
  // Add new commands
  commandMap["ECHO"] = std::bind(bob::printEcho, std::ref(std::cin), std::ref(polyList), std::ref(std::cout));
  commandMap["INFRAME"] = std::bind(bob::printInframe, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["MAXSEQ"] = std::bind(bob::printMaxseq, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["PERMS"] = std::bind(bob::printPerms, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["SAME"] = std::bind(bob::printSame, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));

  // Process commands
  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      if (commandMap.find(cmd) != commandMap.end())
      {
        commandMap.at(cmd)();
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