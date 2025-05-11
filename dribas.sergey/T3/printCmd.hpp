#ifndef PRINTCMD_HPP
#define PRINTCMD_HPP

#include <iostream>
#include <vector>
#include "poligon.hpp"

namespace dribas
{
  std::ostream printArea(const std::vector< Poligon >&, std::istream&);
  std::ostream printMax(const std::vector< Poligon >&, std::istream&);
  std::ostream printMin(const std::vector< Poligon >&, std::istream&);
  std::ostream printCount(const std::vector< Poligon >&, std::istream&);
  std::ostream printLessArea(const std::vector< Poligon >&, std::istream&);
  std::ostream printLessArea(const std::vector< Poligon >&, std::istream&);
}

#endif
