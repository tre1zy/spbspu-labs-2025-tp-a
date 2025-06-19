#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include "commands.hpp"
#include "polygon.hpp"

int main(int argc, char** argv)
{
  if (argc != 2) {
    std::cerr << "Error: filename parameter missing" << std::endl;
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "Error: cannot open file" << std::endl;
    return 2;
  }

  std::vector<amine::Polygon> polygons;
  std::vector<std::string> lines;
  std::copy(std::istream_iterator<std::string>(file),
            std::istream_iterator<std::string>(),
            std::back_inserter(lines));

  std::for_each(lines.begin(), lines.end(),
    [&polygons](const std::string& line) {
      amine::Polygon poly;
      if (amine::parse_polygon(line, poly)) {
        polygons.push_back(poly);
      }
    });

  amine::process_commands(polygons);
  return 0;
}
