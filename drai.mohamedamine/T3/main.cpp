#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include "polygon.hpp"
#include "commands.hpp"

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "Error: missing filename argument\n";
    return 1;
  }

  std::ifstream infile(argv[1]);
  if (!infile)
  {
    std::cerr << "Error: could not open file\n";
    return 1;
  }

  std::stringstream buffer;
  buffer << infile;
  std::string content = buffer.str();

  std::vector< std::string > lines;
  std::string::size_type prev = 0;
  std::string::size_type pos = content.find('\n');

  std::generate_n(
    std::back_inserter(lines),
    std::count(content.begin(), content.end(), '\n') + (content.back() != '\n' ? 1 : 0),
    [&]() {
      std::string line = content.substr(prev, pos - prev);
      prev = (pos == std::string::npos) ? content.size() : pos + 1;
      pos = content.find('\n', prev);
      return line;
    }
  );

  std::vector< amine::Polygon > polygons;
  std::transform(lines.begin(), lines.end(), std::back_inserter(polygons),
    [](const std::string& line)
    {
      amine::Polygon poly;
      if (amine::parse_polygon(line, poly) && poly.points.size() >= 3)
        return poly;
      return amine::Polygon{};
    });

  polygons.erase(
    std::remove_if(polygons.begin(), polygons.end(),
      [](const amine::Polygon& p) { return p.points.empty(); }),
    polygons.end());

  amine::process_commands(polygons);
  return 0;
}
