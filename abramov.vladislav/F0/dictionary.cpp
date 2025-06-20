#include "dictionary.hpp"
#include <ostream>
#include <stdexcept>

abramov::Dictionary::Dictionary():
  dict_(std::unordered_map< std::string, std::string >{})
{}

abramov::Dictionary::Dictionary(const Dictionary &dict):
  dict_(std::unordered_map< std::string, std::string >(dict.dict_))
{}

abramov::Dictionary &abramov::Dictionary::operator=(const Dictionary &dict)
{
  if (this != std::addressof(dict))
  {
    Dictionary tmp(dict);
    swap(tmp);
  }
  return *this;
}

void abramov::Dictionary::addWord(const std::string &word, const std::string &trans)
{
  if (!dict_.insert({ word, trans }).second)
  {
    throw std::runtime_error("Fail to add word\n");
  }
}

void abramov::Dictionary::deleteWord(const std::string &word)
{
  if (!dict_.erase(word))
  {
    throw std::logic_error("There is no such word\n");
  }
}

void abramov::Dictionary::print(std::ostream &out) const noexcept
{
  for (auto it = dict_.cbegin(); it != dict_.cend(); ++it)
  {
    out << it->first << " : " << it->second << "\n";
  }
}

abramov::Dictionary abramov::Dictionary::setWithDict(const Dictionary &dict) const
{
  Dictionary res{};
  for (auto it = dict_.cbegin(); it != dict_.cend(); ++it)
  {
    if (dict.dict_.find(it->first) == dict.dict_.cend())
    {
      if (!res.dict_.insert({ it->first, it->second }).second)
      {
        throw std::runtime_error("Fail to create dictionary\n");
      }
    }
  }
  for (auto it = dict.dict_.cbegin(); it != dict.dict_.cend(); ++it)
  {
    if (dict_.find(it->first) == dict_.cend())
    {
      if (!res.dict_.insert({ it->first, it->second }).second)
      {
        throw std::runtime_error("Fail to create dictionary\n");
      }
    }
  }
  return res;
}

abramov::Dictionary abramov::Dictionary::intersectWithDict(const Dictionary &dict) const
{
  Dictionary res{};
  for (auto it = dict_.cbegin(); it != dict_.cend(); ++it)
  {
    if (dict.dict_.find(it->first) != dict.dict_.cend())
    {
      if (!res.dict_.insert({ it->first, it->second }).second)
      {
        throw std::runtime_error("Fail to create dictionary\n");
      }
    }
  }
  return res;
}

abramov::Dictionary abramov::Dictionary::unionWithDict(const Dictionary &dict) const
{
  Dictionary res{};
  for (auto it = dict_.cbegin(); it != dict_.cend(); ++it)
  {
    if (!res.dict_.insert({ it->first, it->second }).second)
    {
      throw std::runtime_error("Fail to create dictionary\n");
    }
  }
  for (auto it = dict.dict_.cbegin(); it != dict.dict_.cend(); ++it)
  {
    if (res.dict_.find(it->first) == res.dict_.end())
    {
      if (!res.dict_.insert({ it->first, it->second }).second)
      {
        throw std::runtime_error("Fail to create dictionary\n");
      }
    }
  }
  return res;
}

void abramov::Dictionary::diffDict(const Dictionary &dict)
{
  for (auto it = dict.dict_.cbegin(); it != dict.dict_.cend(); ++it)
  {
    dict_.erase(it->first);
  }
}

void abramov::Dictionary::mergeDict(const Dictionary &dict)
{
  for (auto it = dict.dict_.cbegin(); it != dict.dict_.cend(); ++it)
  {
    if (dict_.find(it->first) == dict_.end())
    {
      addWord(it->first, it->second);
    }
  }
}

bool abramov::Dictionary::empty() const noexcept
{
  return dict_.empty();
}

void abramov::Dictionary::swap(Dictionary &dict) noexcept
{
  std::swap(dict_, dict.dict_);
}
