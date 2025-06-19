#ifndef AMINE_COMMANDS_HPP
#define AMINE_COMMANDS_HPP

#include "polygon.hpp"
#include <string>
#include <vector>

namespace amine
{
  void process_command(
    const std::string& name,
    const std::vector<std::string>& args,
    std::vector<Polygon>& polygons
  );
}

#endif
