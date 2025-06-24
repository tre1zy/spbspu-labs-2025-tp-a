#include "commands.hpp"
#include <delimiter.hpp>
#include <streamGuard.hpp>
#include <limits>

void finaev::create_debut(std::istream& in, std::ostream& out, globalDebuts& debuts)
{
  std::string key;
  if (!(in >> key))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (debuts.find(key) != debuts.end())
  {
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    throw std::runtime_error("<DUPLICATE>");
  }
  StreamGuard guard(in);
  OpenningInfo inf;
  if (!(in >> inf.moves))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  in >> std::ws;
  in >> DelimiterIO{'"'};
  std::getline(in, inf.name, '"');
  in >> std::ws;
  in >> DelimiterIO{'"'};
  std::getline(in, inf.description, '"');
  debuts[key] = inf;
  out << "Openning " << key << " successfully added";
}

void finaev::create_base(std::istream& in, std::ostream& out, debutsBases& db)
{
  std::string baseName;
  if (!(in >> baseName))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::string temp;
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (db.find(baseName) != db.end()) 
  {
    throw std::runtime_error("<DUPLICATE>");
  }
  db[baseName] = std::unordered_map<std::string, bool>();
  out << "Base " << baseName << " successfully added";
}

using mapOfCommands = std::map< std::string, std::function< void() > >;
mapOfCommands finaev::createCommandsHandler(std::istream& in, std::ostream& out, globalDebuts& debuts, debutsBases& bases)
{
  mapOfCommands commands;
  commands["create_debut"] = std::bind(finaev::create_debut, std::ref(in), std::ref(out), std::ref(debuts));
  commands["create_base"] = std::bind(finaev::create_base, std::ref(in), std::ref(out), std::ref(bases));
  return commands;
}
