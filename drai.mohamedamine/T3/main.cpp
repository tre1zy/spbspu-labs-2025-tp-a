#include "commands.hpp"
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <string>
#include "polygon.hpp"

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "Error: missing filename argument\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "Error: could not open file\n";
    return 1;
  }

  std::vector< amine::Polygon > polygons;

  std::istream_iterator< std::string > start(file);
  std::istream_iterator< std::string > end;

  std::string line;
  std::vector< std::string > lines;
  std::copy(std::istream_iterator<std::string>(file), std::istream_iterator<std::string>(), std::back_inserter(lines));

  std::transform(lines.begin(), lines.end(), std::back_inserter(polygons),
    [](const std::string& str) {
      amine::Polygon p;
      if (amine::parse_polygon(str, p) && p.points.size() >= 3)
        return p;
      return amine::Polygon(); // пустая
    });

  polygons.erase(
    std::remove_if(polygons.begin(), polygons.end(),
      [](const amine::Polygon& p) { return p.points.empty(); }),
    polygons.end());

  amine::CommandProcessor processor(polygons);

  std::for_each(std::istream_iterator< std::string >(std::cin), std::istream_iterator< std::string >(),
    [&](const std::string& cmd) { processor(cmd); });

  return 0;
}
