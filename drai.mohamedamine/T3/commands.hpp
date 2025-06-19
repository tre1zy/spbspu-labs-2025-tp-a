#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <vector>
#include <string>
#include "polygon.hpp"

namespace amine
{
  void execute_command(const std::string& command, const std::vector<std::string>& args, std::vector<Polygon>& polygons);
  void process_commands(std::vector<Polygon>& polygons);
}

#endif
