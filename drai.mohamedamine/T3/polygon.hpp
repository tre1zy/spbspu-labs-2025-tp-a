#ifndef AMINE_COMMANDS_HPP
#define AMINE_COMMANDS_HPP

#include "polygon.hpp"
#include <vector>
#include <optional>
#include <string>

namespace amine
{
  void process_commands(std::vector<Polygon>& polygons);
  std::optional<Polygon> parse_polygon(const std::vector<std::string>& args);
}

#endif
