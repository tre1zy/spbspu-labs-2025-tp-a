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
  commands["MAXSEQ"] = std::bind(kharlamov::maxSeq, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["SAME"] = std::bind(kharlamov::same, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));

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
