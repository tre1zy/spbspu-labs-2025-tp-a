#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

#include "polygon.hpp"

namespace amine {

int main(int argc, char* argv[])
{
  if (argc < 2) {
    std::cerr << "Error: missing filename argument\n";
    return 1;
  }

  std::ifstream infile(argv[1]);
  if (!infile) {
    std::cerr << "Error: could not open file\n";
    return 1;
  }

  std::vector<std::string> lines;
  std::string line;
  while (std::getline(infile, line)) {
    lines.push_back(line);
  }

  std::vector<Polygon> polygons;
  std::for_each(lines.begin(), lines.end(), [&](const std::string& line) {
    Polygon poly;
    if (!line.empty() && parse_polygon(line, poly) && poly.points.size() >= 3) {
      polygons.emplace_back(std::move(poly));
    }
  });

  process_commands(polygons);
  return 0;
}

}
