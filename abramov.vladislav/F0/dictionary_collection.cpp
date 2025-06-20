#include "dictionary_collection.cpp"
#include <stdexcept>

abramov::DictionaryCollection::DictionaryCollection():
  collection_(std::unordered_map< std::string, Dictionary >{})
{}

void abramov::DictionaryCollection::addDict(const std::string &name)
{
  if (collection_.find(name) == collection_.end())
  {
    if (!collection.insert({ name, Dictionary() }).second)
    {
      throw std::runtime_error("Fail to add dictionary\n");
    }
  }
}

void abramov::DictionaryCollection::deleteDict(const std::string &name)
{
  if (!collection_.erase(name))
  {
    throw std::logic_error("There is no such dictionary\n");
  }
}
