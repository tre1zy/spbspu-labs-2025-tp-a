#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <sstream>
#include "commands.hpp"
#include "polygon.hpp"

int main(int argc, char** argv)
{
  if (argc != 2) {
    std::cerr << "Error: filename parameter missing\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "Error: cannot open file\n";
    return 2;
  }

  std::vector<std::string> lines;
  std::string buffer;
  std::ostringstream oss;

  oss << file.rdbuf();
  buffer = oss.str();

  std::istringstream input(buffer);
  std::copy(
    std::istream_iterator<std::string>(input),
    std::istream_iterator<std::string>(),
    std::back_inserter(lines)
  );

  std::vector<amine::Polygon> polygons;
  std::transform(
    lines.begin(),
    lines.end(),
    std::back_inserter(polygons),
    [](const std::string& line) {
      amine::Polygon poly;
      if (amine::parse_polygon(line, poly)) {
        return poly;
      }
      return amine::Polygon{};
    }
  );

  polygons.erase(
    std::remove_if(
      polygons.begin(),
      polygons.end(),
      [](const amine::Polygon& p) {
        return p.points.empty();
      }
    ),
    polygons.end()
  );

  amine::process_commands(polygons);
  return 0;
}
