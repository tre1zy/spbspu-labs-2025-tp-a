#include "geometry.hpp"
#include "commands.hpp"
#include <iostream>
#include <fstream>
#include <iterator>
#include <limits>
#include <map>
#include <functional>
#include <algorithm>
#include <stdexcept>

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
  if (!inFile.is_open())
  {
    std::cer << "Could not open file" << argv[1] << "\n";
    return 1;
  }

  std::vector< geom::Polygon > polyList;
  Polygon temp;
  while (inFile >> temp)
  {
    polyList.push_back(std::move(temp));
  }

  std::map<std::string, std::function<void()>> commandMap;
  commandMap["AREA"] = std::bind(smirnov::printAreaSum, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["MAX"] = std::bind(smirnov::printMaxValueOf, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["MIN"] = std::bind(smirnov::printMinValueOf, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["COUNT"] = std::bind(smirnov::printCountOf, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["LESSAREA"] = std::bind(smirnov::printLessAreaCnt, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));
  commandMap["INTERSECTIONS"] = std::bind(smirnov::printIntersectionsCnt, std::ref(std::cin), std::cref(polyList), std::ref(std::cout));

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      commandMap.at(cmd)();
      std::cout << '\n';
    }
    catch (const std::out_of_range& e)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    catch (const std::invalid_argument)
    {
      std::cout << "<INVALID COMMAND>\n";
      if (std::cin.fail())
      {
        std::cin.clear();
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      if (std::cin.fail())
      {
        std::cin.clear();
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }

  return 0;
}
