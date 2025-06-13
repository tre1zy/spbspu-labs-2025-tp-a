#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "polygon.hpp"

namespace pilugina
{
  void areaCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void maxCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void minCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void countCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void sameCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
}

#endif
