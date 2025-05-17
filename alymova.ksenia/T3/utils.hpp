#ifndef UTILS_HPP
#define UTILS_HPP
#include <map>
#include <functional>
#include <iostream>
#include "shapes.hpp"

namespace alymova
{
  using CommandDataset = std::map< std::string, std::function< double(const std::vector< Polygon >&) > >;
  using AreaSubcommandDataset = std::map< std::string, std::function< double(double, const Polygon&) > >;

  double area(std::istream& in, const std::vector< Polygon >& polygons);
  double areaEven(double value, const Polygon& polygon);
  double areaOdd(double value, const Polygon& polygon);
  double areaMean(double value, const Polygon& polygon, size_t size);
  double areaNumber(double value, const Polygon& polygon, size_t vertexes);
  double areaOne(const Polygon& polygon);

  void max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);

  bool isDigit(char c);

  CommandDataset complectCommands();
}
#endif
