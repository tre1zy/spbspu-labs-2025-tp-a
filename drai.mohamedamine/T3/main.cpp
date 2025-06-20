#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>

#include "polygon.hpp"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Error: missing filename argument\n";
    return 1;
  }

  std::ifstream infile(argv[1]);
  if (!infile) {
    std::cerr << "Error: could not open file\n";
    return 1;
  }

  std::vector<Polygon> polygons;

  std::vector<std::string> lines{
    std::istream_iterator<std::string>{infile},
    std::istream_iterator<std::string>{}
  };

  std::transform(lines.begin(), lines.end(), std::back_inserter(polygons),
    [](const std::string& line) -> Polygon {
      Polygon poly;
      return (parse_polygon(line, poly) && poly.points.size() >= 3) ? poly : Polygon{};
    });

  polygons.erase(
    std::remove_if(polygons.begin(), polygons.end(),
      [](const Polygon& p) {
        return p.points.empty();
      }),
    polygons.end()
  );

  process_commands(polygons);
  return 0;
}
