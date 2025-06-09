#include "freqDict.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>

namespace kostyukov
{
  bool IsInvalidChar::operator()(char c) const
  {
    return !std::isalnum(c) && c != '_';
  }

  bool isValidName(const std::string& name)
  {
    if (name.empty() || !std::isalpha(name[0]))
    {
      return false;
    }
    return std::find_if(name.begin(), name.end(), IsInvalidChar{}) == name.end();
  }


  void createDict(std::istream& in, std::ostream& out, FreqDictManager& dicts)
  {
    std::string dict_name;
    in >> dict_name;
    if (!in || dict_name.empty())
    {
      out << "<MISSING ARGUMENTS>";
      return;
    }
    if(!isValidName(dict_name))
    {
      out << "INVALID DICTIONARY NAME>";
      return;
    }
    if (dicts.count(dict_name))
    {
      out << "<DICTIONARY EXISTS>";
      return;
    }
    dicts.emplace(dict_name, FrequencyDictionary{});
  }

  void deleteDict(std::istream& in, std::ostream& out, FreqDictManager& dicts)
  {
    std::string dict_name;
    in >> dict_name;
    if (!in || dict_name.empty())
    {
      out << "<MISSING ARGUMENTS>";
      return;
    }
    if (!isValidName(dict_name))
    {
      out << "<INVALID DICTIONARY NAME>";
      return;
    }
    auto iter = dicts.find(dict_name);
    if (iter == dicts.end())
    {
      out << "<DICTIONARY NOT FOUND>";
      return;
    }
    dicts.erase(iter);
  }
}
