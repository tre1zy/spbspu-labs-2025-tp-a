#include "commands.hpp"
#include <string>
#include <algorithm>
#include <iterator>

namespace
{
  const std::string& extractDict(const kiselev::Dicts::value_type& val)
  {
    return val.first;
  }
}
void kiselev::doNewDict(std::istream& in, std::ostream& out, Dicts& dicts)
{
  std::string newDict;
  in >> newDict;
  if (dicts.find(newDict) != dicts.end())
  {
    out << "<DICTIONARY ALREADY EXISTS>\n";
    return;
  }
  dicts[newDict];
}

void kiselev::doDeleteDict(std::istream& in, std::ostream& out, Dicts& dicts)
{
  std::string dict;
  in >> dict;
  if (dicts.find(dict) == dicts.end())
  {
    out << "<DICTIONARY NOT FOUND>\n";
    return;
  }
  dicts.erase(dict);
}

void kiselev::doAddElement(std::istream& in, std::ostream& out, Dicts& dicts)
{
  std::string nameDict;
  std::string eng;
  std::string rus;
  in >> nameDict >> eng >> rus;
  auto dictIt = dicts.find(nameDict);
  if (dictIt == dicts.end())
  {
    out << "<DICTIONARY NOT FOUND>\n";
    return;
  }
  Dict dict = dictIt->second;
  auto engIt = dict.find(eng);
  if (engIt == dict.end())
  {
    std::vector< std::string > translations;
    translations.push_back(rus);
    dict[eng] = translations;
  }
  else
  {
    engIt->second.push_back(rus);
  }
}

void kiselev::doDeleteElement(std::istream& in, std::ostream& out, Dicts& dicts)
{
  std::string nameDict;
  std::string eng;
  in >> nameDict >> eng;
  auto dictIt = dicts.find(nameDict);
  if (dictIt == dicts.end())
  {
    out << "<DICTIONARY NOT FOUND>\n";
    return;
  }
  Dict dict = dictIt->second;
  auto engIt = dict.find(eng);
  if (engIt == dict.end())
  {
    out << "<ENGWORD NOT FOUND>\n";
    return;
  }
  dict.erase(eng);
}

void kiselev::doListDict(std::ostream& out, const Dicts& dicts)
{
  if (dicts.empty())
  {
    out << "<EMPTY>\n";
    return;
  }
  std::transform(dicts.begin(), dicts.end(), std::ostream_iterator<std::string>(out, "\n"), extractDict);
}
