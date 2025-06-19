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

  std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  std::istringstream stream(content);
  std::vector<std::string> lines;

  std::generate_n(
    std::back_inserter(lines),
    std::count(content.begin(), content.end(), '\n') + 1,
    [&stream]() {
      std::string line;
      std::getline(stream, line);
      return line;
    }
  );

  std::vector<amine::Polygon> polygons;
  std::transform(
    lines.begin(),
    lines.end(),
    std::back_inserter(polygons),
    [](const std::string& line) {
      amine::Polygon p;
      return amine::parse_polygon(line, p) ? p : amine::Polygon{};
    }
  );

  polygons.erase(
    std::remove_if(polygons.begin(), polygons.end(), [](const amine::Polygon& p) {
      return p.points.empty();
    }),
    polygons.end()
  );

  amine::process_commands(polygons);
  return 0;
}
