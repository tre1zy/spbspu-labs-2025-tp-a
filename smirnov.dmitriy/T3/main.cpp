#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>
#include <limits>
#include <functional>
#include <iterator>
#include "polygon.hpp"
#include "commands.hpp"

int main()
{
  using namespace smirnov;

  std::vector< Polygon > polygons;
  Polygon poly;
  while (std::cin >> poly)
  {
    polygons.push_back(poly);
  }

  std::cin.clear();
  std::string cmd;

  while (std::cin >> cmd)
  {
    try
    {
      if (cmd == "AREA")
      {
        doAreaCommand(polygons, std::cin, std::cout);
      }
      else if (cmd == "COUNT")
      {
        doCountCommand(polygons, std::cin, std::cout);
      }
      else if (cmd == "MAX")
      {
        doMaxCommand(polygons, std::cin, std::cout);
      }
      else if (cmd == "MIN")
      {
        doMinCommand(polygons, std::cin, std::cout);
      }
      else if (cmd == "RECTS")
      {
        doRectsCommand(polygons, std::cout);
      }
      else if (cmd == "MAXSEQ")
      {
        doMaxseqCommand(polygons, std::cin, std::cout);
      }
      else if (cmd == "INTERSECTIONS")
      {
        doIntersections(polygons, std::cin, std::cout);
      }
      else
      {
        throw std::logic_error("INVALID COMMAND");
      }
    }
    catch (const std::logic_error& e)
    {
      std::cout << "<" << e.what() << ">" << '\n';
      std::cin.clear();
      std::string tmp;
      std::getline(std::cin, tmp);
    }
  }

  return 0;
}

