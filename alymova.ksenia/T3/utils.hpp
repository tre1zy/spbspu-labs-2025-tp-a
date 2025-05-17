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
  void maxAndMin(const MaxMinSubcommands& subs, std::istream& in,
    std::ostream& out,const std::vector< Polygon >& polygons);
  void count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void inFrame(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);

  CommandDataset complectCommands();
}
#endif
