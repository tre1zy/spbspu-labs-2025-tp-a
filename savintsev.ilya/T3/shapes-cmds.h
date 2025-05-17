#ifndef SHAPES_CMDS_H
#define SHAPES_CMDS_H
#include <iostream>
#include <string>
#include "shapes.h"

namespace savintsev
{
  void area(std::istream & in, const std::vector< Polygon > & data)
  {
    std::string subcommand;
    in >> subcommand;
    if (subcommand == "EVEN")
    {
      data.size();
    }
  }
  void print(std::ostream & out, const std::vector< Polygon > & data)
  {
    for (size_t i = 0; i < data.size(); ++i)
    {
      out << data[i] << '\n';
    }
  }
}

#endif
