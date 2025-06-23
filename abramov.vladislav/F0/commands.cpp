#include "commands.hpp"
#include <vector>
#include <iostream>
#include <stdexcept>

void abramov::getCommands(std::map< std::string, std::function< void() > > &commands, DictionaryCollection &collect)
{
  commands["adddict"] = std::bind(addDict, std::ref(collect), std::ref(std::cin));
  commands["addtodict"] = std::bind(addToDict, std::ref(collect), std::ref(std::cin));
  commands["rmdict"] = std::bind(deleteDict, std::ref(collect), std::ref(std::cin));
  commands["rmfromdict"] = std::bind(deleteFromDict, std::ref(collect), std::ref(std::cin));
  commands["set"] = std::bind(setDicts, std::ref(collect), std::ref(std::cin));
  commands["intersection"] = std::bind(intersectDicts, std::ref(collect), std::ref(std::cin));
  commands["union"] = std::bind(unionDicts, std::ref(collect), std::ref(std::cin));
  commands["difference"] = std::bind(diffDicts, std::ref(collect), std::ref(std::cin));
  commands["merge"] = std::bind(mergeDicts, std::ref(collect), std::ref(std::cin));
  commands["print"] = std::bind(printDict, std::cref(collect), std::ref(std::cin), std::ref(std::cout));
}

void abramov::addDict(DictionaryCollection &collect, std::istream &in)
{
  std::string name;
  in >> name;
  if (name.empty())
  {
    throw std::logic_error("There are no enough arguments\n");
  }
  collect.addDict(name);
}

void abramov::addToDict(DictionaryCollection &collect, std::istream &in)
{
  std::string name;
  std::string word;
  std::string trans;
  in >> name >> word >> trans;
  if (name.empty() || word.empty() || trans.empty())
  {
    throw std::logic_error("There are no enough arguments\n");
  }
  Dictionary &dict = collect.findDict(name);
  dict.addWord(word, trans);
}

void abramov::deleteDict(DictionaryCollection &collect, std::istream &in)
{
  std::string name;
  in >> name;
  if (name.empty())
  {
    throw std::logic_error("There are no enough arguments\n");
  }
  collect.deleteDict(name);
}

void abramov::deleteFromDict(DictionaryCollection &collect, std::istream &in)
{
  std::string name;
  std::string word;
  in >> name >> word;
  if (name.empty() || word.empty())
  {
    throw std::logic_error("There is no enough arguments\n");
  }
  Dictionary &dict = collect.findDict(name);
  dict.deleteWord(word);
}

void abramov::setDicts(DictionaryCollection &collect, std::istream &in)
{
  std::string new_name;
  in >> new_name;
  char bracket = '\0';
  in >> bracket;
  if (bracket != '(')
  {
    throw std::logic_error("Expected '('\n");
  }
  std::string name;
  Dictionary res{};
  bool first = true;
  while (in >> name && name != ")")
  {
    if (first)
    {
      res = collect.findDict(name);
      first = false;
    }
    else
    {
      res = res.setWithDict(collect.cfindDict(name));
    }
  }
  if (name != ")")
  {
    throw std::logic_error("Expected ')'\n");
  }
  collect.addCompleteDict(new_name, res);
}

void abramov::intersectDicts(DictionaryCollection &collect, std::istream &in)
{
  std::string new_name;
  in >> new_name;
  char bracket = '\0';
  in >> bracket;
  if (bracket != '(')
  {
    throw std::logic_error("Expected '('\n");
  }
  Dictionary res{};
  std::string name;
  bool first = true;
  while (in >> name && name != ")")
  {
    if (first)
    {
      res = collect.findDict(name);
      first = false;
    }
    else
    {
      res = res.intersectWithDict(collect.cfindDict(name));
    }
  }
  if (name != ")")
  {
    throw std::logic_error("Expected ')'\n");
  }
  collect.addCompleteDict(new_name, res);
}

void abramov::unionDicts(DictionaryCollection &collect, std::istream &in)
{
  std::string new_name;
  in >> new_name;
  char bracket = '\0';
  in >> bracket;
  if (bracket != '(')
  {
    throw std::logic_error("Expected '('\n");
  }
  std::string name;
  Dictionary res{};
  bool first = true;
  while (in >> name && name != ")")
  {
    if (first)
    {
      res = collect.findDict(name);
      first = false;
    }
    else
    {
      res = res.unionWithDict(collect.cfindDict(name));
    }
  }
  if (name != ")")
  {
    throw std::logic_error("Expected ')'\n");
  }
  collect.addCompleteDict(new_name, res);
}

void abramov::diffDicts(DictionaryCollection &collect, std::istream &in)
{
  std::string new_name;
  in >> new_name;
  char bracket = '\0';
  in >> bracket;
  if (bracket != '(')
  {
    throw std::logic_error("Expected '('\n");
  }
  std::string name;
  Dictionary res{};
  bool first = true;
  while (in >> name && name != ")")
  {
    if (first)
    {
      res = collect.findDict(name);
      first = false;
    }
    else
    {
      res = res.diffDict(collect.cfindDict(name));
    }
  }
  if (name != ")")
  {
    throw std::logic_error("Expected ')'\n");
  }
  collect.addCompleteDict(new_name, res);
}

void abramov::mergeDicts(DictionaryCollection &collect, std::istream &in)
{
  std::string new_name;
  in >> new_name;
  char bracket = '\0';
  in >> bracket;
  if (bracket != '(')
  {
    throw std::logic_error("Expected '('\n");
  }
  std::string name;
  Dictionary res{};
  bool first = true;
  std::vector< std::string > del;
  while (in >> name && name != ")")
  {
    if (first)
    {
      res = collect.findDict(name);
      first = false;
      del.push_back(name);
    }
    else
    {
      res = res.mergeDict(collect.cfindDict(name));
      del.push_back(name);
    }
  }
  if (name != ")")
  {
    throw std::logic_error("Expected ')'\n");
  }
  collect.addCompleteDict(new_name, res);
  for (const auto &dict_name : del)
  {
    collect.deleteDict(dict_name);
  }
}

void abramov::printDict(const DictionaryCollection &collect, std::istream &in, std::ostream &out)
{
  std::string name;
  in >> name;
  if (name.empty())
  {
    throw std::logic_error("There are no enough arguments\n");
  }
  const Dictionary &dict = collect.cfindDict(name);
  dict.print(out);
}

