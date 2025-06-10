#include "commands.h"
#include "polygon.h"
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <vector>

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  std::vector<kharlamov::Polygon> polygons;
  kharlamov::Polygon poly;
  while (file >> poly)
  {
    polygons.push_back(poly);
  }
  std::map<std::string, std::function<void()>> commands;
  commands["AREA"] = std::bind(kharlamov::doAreaCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MAX"] = std::bind(kharlamov::doMaxCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MIN"] = std::bind(kharlamov::doMinCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["COUNT"] = std::bind(kharlamov::doCountCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MAXSEQ"] = std::bind(kharlamov::domaxSeqCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["SAME"] = std::bind(kharlamov::dosameCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      commands.at(cmd)();
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  return 0;
}
