#ifndef LINE_COMMANDS_HPP
#define LINE_COMMANDS_HPP
#include "geometric_shape.hpp"
#include <iostream>
#include <list>

namespace zakirov
{
  void process_area(const std::list< Polygon > & points, std::istream & in, std::ostream & out);
  void process_less_area(const std::list< Polygon > & points, std::istream & in, std::ostream & out);
  void find_extremum_max(const std::list< Polygon > & points, std::istream & in, std::ostream & out);
  void find_extremum_min(const std::list< Polygon > & points, std::istream & in, std::ostream & out);
  void count(const std::list< Polygon > & points, std::istream & in, std::ostream & out);
  void echo(std::list< Polygon > & points, std::istream & in, std::ostream & out);
}

#endif
