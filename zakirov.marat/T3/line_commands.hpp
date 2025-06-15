#ifndef LINE_COMMANDS_HPP
#define LINE_COMMANDS_HPP
#include "geometric_shape.hpp"

namespace zakirov
{
  float count_area(const Polygon & polygon);
  void process_area(const std::list< Polygon > & points, std::istream & in, std::ostream & out);
  void find_extremum(const std::list< Polygon > & points, std::istream & in, std::ostream & out);
  void count(const std::list< Polygon > & points, std::istream & in, std::ostream & out);
  void echo(std::list< Polygon > & points, std::istream & in, std::ostream & out);
}

#endif
