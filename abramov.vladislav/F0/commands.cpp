#include "commands.hpp"
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
  dict.deleteWord(name);
}

void abramov::setDicts(DictionaryCollection &collect, std::istream &in)
{
  std::string name;
  in >> name;
  if (name.empty())
  {
    throw std::logic_error("There is no enough arguments\n");
  }
  Dictionary res = Dictionary(collect.findDict(name));
  while (in >> name)
  {
    if (name.empty())
    {
      continue;
    }
    if (in.peek() == '\n')
    {
      break;
    }
    res = res.setWithDict(collect.cfindDict(name));
  }
}

void abramov::intersectDicts(DictionaryCollection &collect, std::istream &in)
{
  std::string name;
  in >> name;
  if (name.empty())
  {
    throw std::logic_error("There is no enough arguments\n");
  }
  Dictionary res = Dictionary(collect.findDict(name));
  while (in >> name)
  {
    if (name.empty())
    {
      continue;
    }
    if (in.peek() == '\n')
    {
      break;
    }
    res = res.intersectWithDict(collect.cfindDict(name));
  }
}

void abramov::unionDicts(DictionaryCollection &collect, std::istream &in)
{
  std::string name;
  in >> name;
  if (name.empty())
  {
    throw std::logic_error("There is no enough arguments\n");
  }
  Dictionary res = Dictionary(collect.findDict(name));
  while (in >> name)
  {
    if (name.empty())
    {
      continue;
    }
    if (in.peek() == '\n')
    {
      break;
    }
    res = res.unionWithDict(collect.cfindDict(name));
  }
}

void abramov::diffDicts(DictionaryCollection &collect, std::istream &in)
{
  std::string name;
  in >> name;
  if (name.empty())
  {
    throw std::logic_error("There is no enough arguments\n");
  }
  Dictionary &dict = collect.findDict(name);
  while (in >> name)
  {
    if (name.empty())
    {
      continue;
    }
    if (in.peek() == '\n')
    {
      break;
    }
    const Dictionary &sub_dict = collect.cfindDict(name);
    dict.diffDict(sub_dict);
  }
}

void abramov::mergeDicts(DictionaryCollection &collect, std::istream &in)
{
  std::string name;
  in >> name;
  if (name.empty())
  {
    throw std::logic_error("There is no enough arguments\n");
  }
  Dictionary &dict = collect.findDict(name);
  while (in >> name)
  {
    if (name.empty())
    {
      continue;
    }
    if (in.peek() == '\n')
    {
      break;
    }
    const Dictionary &sub_dict = collect.cfindDict(name);
    dict.mergeDict(sub_dict);
    collect.deleteDict(name);
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

