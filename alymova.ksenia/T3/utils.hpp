#ifndef UTILS_HPP
#define UTILS_HPP
#include <map>
#include <functional>
#include <iostream>
#include "shapes.hpp"

namespace alymova
{
  using CommandDataset = std::map< std::string, std::function< void(const std::vector< Polygon >&) > >;
  using AreaSubcommands = std::map< std::string, std::function< double(double, const Polygon&) > >;
  using MaxMinSubcommands = std::map< std::string, std::function< double(double, const Polygon&) > >;
  using CountSubcommands = std::map< std::string, std::function< bool(const Polygon&) > >;

  void area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  double areaEven(double value, const Polygon& polygon);
  double areaOdd(double value, const Polygon& polygon);
  double areaMean(double value, const Polygon& polygon, size_t size);
  double areaNumber(double value, const Polygon& polygon, size_t vertexes);
  double areaPolygon(const Polygon& polygon);

  void maxAndMin(const MaxMinSubcommands& subs, std::istream& in,
    std::ostream& out,const std::vector< Polygon >& polygons);
  double compareMaxArea(double value, const Polygon& polygon);
  size_t compareMaxVertexes(size_t value, const Polygon& polygon);
  double compareMinArea(double value, const Polygon& polygon);
  size_t compareMinVertexes(size_t value, const Polygon& polygon);
  void count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);

  bool isDigit(char c);
  bool isEqualSize(size_t size, const Polygon& polygon);
  size_t getVertexes(std::string str);

  CommandDataset complectCommands();
}
#endif
