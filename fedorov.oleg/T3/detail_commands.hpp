#ifndef DETAIL_COMMANDS_HPP
#define DETAIL_COMMANDS_HPP

#include <iostream>

namespace fedorov
{
  bool isEOL(std::istream &in);

  void outputFormattedArea(double value, std::ostream &out);
}

#endif
