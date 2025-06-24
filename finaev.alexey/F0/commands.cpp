#include "commands.hpp"
#include <delimiter.hpp>
#include <streamGuard.hpp>

void finaev::create_debut(std::istream& in, std::ostream& out, globalDebuts& debuts)
{
  std::string key;
  if (!(in >> key))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (debuts.find(key) != debuts.end())
  {
    out << "<DUPLICATE>";
    return;
  }
  StreamGuard guard(in);
  OpenningInfo inf;
  if (!(in >> inf.moves))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (!(in >> inf.name))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (!(in >> inf.description))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  debuts[key] = inf;
  out << "openning " << key << " successfully added";
}

using mapOfCommands = std::map< std::string, std::function< void() > >;
mapOfCommands finaev::createCommandsHandler(std::istream& in, std::ostream& out, globalDebuts& debuts/*, debutsBase& bases*/)
{
  mapOfCommands commands;
  commands["create_debut"] = std::bind(finaev::create_debut, std::ref(in), std::ref(out), std::ref(debuts));
  return commands;
}
