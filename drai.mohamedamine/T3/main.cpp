#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "polygon.hpp"

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

  std::vector< amine::Polygon > polygons;
  std::string line;
  amine::Polygon poly;

  std::getline(infile, line);
  if (!line.empty() && amine::parse_polygon(line, poly) && poly.points.size() >= 3)
  {
    polygons.push_back(poly);
  }

  std::getline(infile, line);
  poly = amine::Polygon{};
  if (!line.empty() && amine::parse_polygon(line, poly) && poly.points.size() >= 3)
  {
    polygons.push_back(poly);
  }

  std::getline(infile, line);
  poly = amine::Polygon{};
  if (!line.empty() && amine::parse_polygon(line, poly) && poly.points.size() >= 3)
  {
    polygons.push_back(poly);
  }

  std::getline(infile, line);
  poly = amine::Polygon{};
  if (!line.empty() && amine::parse_polygon(line, poly) && poly.points.size() >= 3)
  {
    polygons.push_back(poly);
  }
  std::getline(infile, line);
  poly = amine::Polygon{};
  if (!line.empty() && amine::parse_polygon(line, poly) && poly.points.size() >= 3)
  {
    polygons.push_back(poly);
  }

  amine::CommandProcessor processor(polygons);

  std::string command;
  std::getline(std::cin, command);
  processor(command);

  std::getline(std::cin, command);
  processor(command);
  return 0;
}
