
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
  {
    std::ifstream inFile(argv[1]);
    if (inFile)
    {
     
      std::copy(it(inFile), it(), std::back_inserter(polyList));
    }
  }

  
  using CommandFn = std::function<void(std::istream&, const std::vector<Polygon>&, std::ostream&)>;
  std::unordered_map<std::string, CommandFn> commandMap;

  
  commandMap["AREA"] = [](std::istream& in, const auto& polygons, std::ostream& out){ bob::printAreaSum(in, polygons, out); };
  commandMap["COUNT"] = [](std::istream& in, const auto& polygons, std::ostream& out){ bob::printCountOf(in, polygons, out); };
  commandMap["MAXSEQ"] = [](std::istream& in, const auto& polygons, std::ostream& out){ bob::printMaxSeq(in, polygons, out); };
  commandMap["ECHO"] = [](std::istream& in, const auto& polygons, std::ostream& out){ bob::printEcho(in, polygons, out); };
  commandMap["INFRAME"] = [](std::istream& in, const auto& polygons, std::ostream& out){ bob::printInframe(in, polygons, out); };
  commandMap["PERMS"] = [](std::istream& in, const auto& polygons, std::ostream& out){ bob::printPerms(in, polygons, out); };
  commandMap["SAME"] = [](std::istream& in, const auto& polygons, std::ostream& out){ bob::printSame(in, polygons, out); };
  commandMap["LESSAREA"] = [](std::istream& in, const auto& polygons, std::ostream& out){ bob::printLessAreaCnt(in, polygons, out); };
  commandMap["INTERSECTIONS"] = [](std::istream& in, const auto& polygons, std::ostream& out){ bob::printIntersectionsCnt(in, polygons, out); };
  commandMap["MAX"] = [](std::istream& in, const auto& polygons, std::ostream& out){ bob::printMaxValueOf(in, polygons, out); };
  commandMap["MIN"] = [](std::istream& in, const auto& polygons, std::ostream& out){ bob::printMinValueOf(in, polygons, out); };
  commandMap["LESS"] = [](std::istream& in, const auto& polygons, std::ostream& out){ bob::printLess(in, polygons, out); };
 

  std::string cmd;
  
  while (std::cin >> cmd)
  {
   
    std::string restOfLine;
    std::getline(std::cin, restOfLine); 
   
    std::istringstream argsStream(restOfLine);

    auto itCmd = commandMap.find(cmd);
    if (itCmd == commandMap.end())
    {
     
      std::cout << "<INVALID COMMAND>\n";
      continue;
    }

    try
    {
      
      itCmd->second(argsStream, polyList, std::cout);
      
      
      std::cout << "\n";
    }
    catch (const std::exception& )
    {
      
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
