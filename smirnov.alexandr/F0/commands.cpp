#include "commands.hpp"

void smirnov::createCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string name;
  in >> name;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (dicts.find(name) != dicts.end())
  {
    out << "The dictionary with name " << name << " already exists.\n";
    return;
  }
  dicts[name] = Dict();
  out << name << " is successfully created.\n";
}
