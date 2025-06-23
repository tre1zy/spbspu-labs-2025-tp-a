#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <functional>
#include <string>
#include <limits>
#include "polygon.hpp"
#include "commands.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Error: missing filename argument\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "Error: cannot open file\n";
    return 1;
  }

  std::vector<averenkov::Polygon> polygons;
  averenkov::Polygon poly;
  while (file >> poly)
  {
    polygons.push_back(poly);
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  std::map<std::string, std::function<void()>> cmds = {
    {"AREA", std::bind(averenkov::printAreaSum, std::ref(std::cin), std::cref(polygons), std::ref(std::cout))},
    {"MAX", std::bind(averenkov::printMaxValueOf, std::ref(std::cin), std::cref(polygons), std::ref(std::cout))},
    {"MIN", std::bind(averenkov::printMinValueOf, std::ref(std::cin), std::cref(polygons), std::ref(std::cout))},
    {"COUNT", std::bind(averenkov::printCountOf, std::ref(std::cin), std::cref(polygons), std::ref(std::cout))},
    {"PERMS", std::bind(averenkov::printPermsCnt, std::ref(std::cin), std::cref(polygons), std::ref(std::cout))},
    {"RIGHTSHAPES", std::bind(averenkov::printRightsCnt, std::cref(polygons), std::ref(std::cout))},
    {"RMECHO", std::bind(averenkov::printRmEcho, std::ref(std::cin), std::ref(polygons), std::ref(std::cout))}
  };

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      cmds.at(command)();
      std::cout << '\n';
    }
    catch (const std::exception&)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
