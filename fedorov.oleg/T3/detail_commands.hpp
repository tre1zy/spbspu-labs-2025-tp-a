#ifndef DETAIL_COMMANDS_HPP
#define DETAIL_COMMANDS_HPP

#include <iostream>
#include <vector>

#include "polygon.hpp"
#include "format_guard.hpp"

namespace fedorov
{
  bool isEOL(std::istream &in);

  void outputFormattedArea(double value, std::ostream &out);
}

#endif
