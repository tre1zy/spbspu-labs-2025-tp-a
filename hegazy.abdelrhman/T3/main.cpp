#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <functional>
#include <algorithm>
#include <string>
#include "geometry.hpp"
#include "commands.hpp"

int main(int argc, char* argv[])
{
  using Polygon = geom::Polygon;

  if (argc != 2)
  {
    return 1;
  }

  std::vector<Polygon> polyList;
  std::ifstream inFile(argv[1]);
  if (!inFile.is_open())
  {
    return 1;
  }

  std::string line;
  while (std::getline(inFile, line))
  {
    if (line.empty()) continue;
    std::istringstream iss(line);
    Polygon poly;
    if (iss >> poly && iss.eof())
    {
      polyList.push_back(poly);
    }
  }
  inFile.close();

  std::map<std::string, std::function<void(std::istream&, const std::vector<Polygon>&, std::ostream&)>> commandMap;
  commandMap["AREA"] = bob::printAreaSum;
  commandMap["MAX"] = bob::printMaxValueOf;
  commandMap["MIN"] = bob::printMinValueOf;
  commandMap["COUNT"] = bob::printCountOf;
  commandMap["LESSAREA"] = bob::printLessAreaCnt;
  commandMap["INTERSECTIONS"] = bob::printIntersectionsCnt;

  std::string cmdLine;
  while (std::getline(std::cin, cmdLine))
  {
    if (cmdLine.empty()) continue;
    std::istringstream iss(cmdLine);
    std::string cmd;
    if (!(iss >> cmd)) continue;
    auto it = commandMap.find(cmd);
    if (it == commandMap.end())
    {
      std::cout << "<INVALID COMMAND>\n";
      continue;
    }
    try
    {
      it->second(iss, polyList, std::cout);
      std::cout << "\n";
    }
    catch (const std::exception&)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
  return 0;
}
