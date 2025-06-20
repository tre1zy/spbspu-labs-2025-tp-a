#include "commands.hpp"
#include <iostream>

void alymova::create(std::istream& in, DictSet& set)
{
  std::string name;
  in >> name;
  if (!in)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  if (set.find(name) != set.end())
  {
    throw std::logic_error("<ALREADY CREATED>");
  }
  set.emplace(name);
}

alymova::DictSet alymova::readDictionaryFile(std::istream& in)
{
  DictSet dataset_comp;
  std::string name;
  while (in >> name)
  {
    Dictionary dataset;
    std::string key, value;
    while (in.peek() != '\n' && in)
    {
      in >> key >> value;
      dataset[key] = value;
    }
    dataset_comp[name] = dataset;
  }
  if ((in).fail() && !(in).eof())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  return dataset_comp;
}


