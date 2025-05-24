#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <vector>
#include <iostream>
#include "polygon.hpp"

namespace trukhanov
{
  using Polygon_vector = std::vector < Polygon >;
  void area(std::istream& in, std::ostream& out, const Polygon_vector& src);
  void max(std::istream& in, std::ostream& out, const Polygon_vector& src);
  void min(std::istream& in, std::ostream& out, const Polygon_vector& src);
  void count(std::istream& in, std::ostream& out, const Polygon_vector& src);
  void lessArea(std::istream& in, std::ostream& out, const Polygon_vector& src);
  void right(std::istream& in, std::ostream& out, const Polygon_vector& src);
}

#endif
