#include "sequence-commands.hpp"
#include <iostream>
#include <limits>
#include <functional>
#include <map>

void aleksandrov::printHelp(std::ostream& out)
{
  out << "No help\n";
}

void aleksandrov::processCommands(std::istream& in, std::ostream& out)
{
  std::map< std::string, std::function< void() > > commands;

  std::string command;
  while (!(in >> command).eof())
  {
    try
    {
      commands.at(command)();
    }
    catch (const std::out_of_range&)
    {
      out << "ERROR: Invalid command!\n";
      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}

