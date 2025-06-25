#include "commands.hpp"
#include <delimiter.hpp>
#include <streamGuard.hpp>
#include <limits>

namespace
{
  bool isExactMoves(const finaev::DebutMoves& f, const finaev::DebutMoves& s)
  {
    return f.moves == s.moves;
  }

  struct СompareMoves
  {
    const finaev::DebutMoves& mov;
    const finaev::globalDebuts& globalOpenings;

    bool operator()(const std::pair< std::string, bool >& base)
    {
      auto it = globalOpenings.find(base.first);
      return it != globalOpenings.end() && isExactMoves(it->second.moves, mov);
    }
  };

  struct ResultCollectorForFind
  {
    const finaev::globalDebuts& debuts;
    std::vector< std::pair< std::string, std::string > >& results;
    const finaev::DebutMoves& mov;
    void operator()(const std::pair< std::string, bool >& baseEntry) const
    {
      auto debut = debuts.find(baseEntry.first);
      if (debut != debuts.end() && debut->second.moves.containsSequence(mov.moves))
      {
        results.emplace_back(baseEntry.first, debut->second.name);
      }
    }
  };

  struct KeySorterForFind
  {
    bool operator()(const std::pair< std::string, std::string >& a, const std::pair< std::string, std::string >& b) const
    {
      return a.first < b.first;
    }
  };

  struct OutputFormatForFind
  {
    std::ostream& out;
    void operator()(const std::pair< std::string, std::string >& pair) const
    {
      static bool first = true;
      if (!first)
      {
        out << "\n";
      }
      out << "- " << pair.first << ": " << pair.second;
      first = false;
    }
  };

  struct OutputFormatForPrint
  {
    std::ostream& out;
    const finaev::globalDebuts& debuts;
    bool first = true;

    void operator()(const std::pair< std::string, bool >& base)
    {
      auto it = debuts.find(base.first);
      if (it != debuts.end())
      {
        if (!first)
        {
          out << "\n";
        }
        out << "- " << it->first << ": " << it->second.name;
        first = false;
      }
    }
  };
}

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

void finaev::add(std::istream& in, std::ostream& out, const globalDebuts& debuts, debutsBases& bases)
{
  std::string baseName, debut;
  if (!(in >> baseName >> debut))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::string temp;
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (debuts.find(debut) == debuts.end())
  {
    throw std::runtime_error("<OPENNING_NOT_FOUND>");
  }
  if (bases.find(baseName) == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  auto& base = bases[baseName];
  if (base.find(debut) != base.end())
  {
    throw std::runtime_error("<DUPLICATE>");
  }
  base[debut] = true;
  out << "Debut " << debut <<  " successfully added in base " << baseName;
}

void finaev::exact_find(std::istream& in, std::ostream& out, const globalDebuts& debuts, const debutsBases& bases)
{
  std::string baseName;
  if (!(in >> baseName))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  DebutMoves mov;
  in >> mov;
  std::string temp;
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (bases.find(baseName) == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  const auto& base = bases.at(baseName);
  СompareMoves pred{ mov, debuts };
  auto found = std::find_if(base.begin(), base.end(), pred);
  if (found != base.end())
  {
    const auto& opening = debuts.at(found->first);
    out << "Key: " << found->first << "\n";
    out << "Name: " << opening.name;
  }
  else
  {
    throw std::runtime_error("<OPENNING_NOT_FOUND>");
  }
}

void finaev::find(std::istream& in, std::ostream& out, const globalDebuts& debuts, const debutsBases& bases)
{
  std::string baseName;
  if (!(in >> baseName))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  DebutMoves mov;
  in >> mov;
  std::string temp;
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (bases.find(baseName) == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  auto base = bases.find(baseName);
  std::vector< std::pair< std::string, std::string > > foundOpenings;
  ResultCollectorForFind collector{ debuts, foundOpenings, mov};
  std::for_each(base->second.begin(), base->second.end(), collector);
  std::sort(foundOpenings.begin(), foundOpenings.end(), KeySorterForFind());
  if (foundOpenings.empty())
  {
    throw std::runtime_error("<OPENNING_NOT_FOUND>");
  }
  out << "Found " << foundOpenings.size() << " openings:\n";
  std::for_each(foundOpenings.begin(), foundOpenings.end(), OutputFormatForFind{ out });
}

void finaev::print(std::istream& in, std::ostream& out, const globalDebuts& debuts, const debutsBases& bases)
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
  auto base = bases.find(baseName);
  if (base == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  OutputFormatForPrint printer{ out, debuts };
  std::for_each(base->second.begin(), base->second.end(), printer);
}

finaev::mapOfCommands finaev::createCommandsHandler(std::istream& in, std::ostream& out, globalDebuts& debuts, debutsBases& bases)
{
  mapOfCommands commands;
  commands["create_debut"] = std::bind(finaev::create_debut, std::ref(in), std::ref(out), std::ref(debuts));
  commands["create_base"] = std::bind(finaev::create_base, std::ref(in), std::ref(out), std::ref(bases));
  commands["add"] = std::bind(finaev::add, std::ref(in), std::ref(out), std::cref(debuts), std::ref(bases));
  commands["exact_find"] = std::bind(finaev::exact_find, std::ref(in), std::ref(out), std::cref(debuts), std::cref(bases));
  commands["find"] = std::bind(finaev::find, std::ref(in), std::ref(out), std::cref(debuts), std::cref(bases));
  commands["print"] = std::bind(finaev::print, std::ref(in), std::ref(out), std::cref(debuts), std::cref(bases));
  return commands;
}
