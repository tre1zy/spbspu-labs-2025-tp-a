#include "commands.hpp"
#include "support.hpp"

void filonova::createDictionary(DictionarySet &dicts, std::istream &in, std::ostream &out)
{
  std::string name;
  in >> name;

  if (!isValidName(name))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  dicts[name].clear();
}

void filonova::deleteDictionary(DictionarySet &dicts, std::istream &in, std::ostream &out)
{
  std::string name;
  in >> name;

  if (dicts.erase(name) == 0)
  {
    out << "<WRONG DICT>\n";
  }
}

void filonova::insert(DictionarySet &dicts, std::istream &in, std::ostream &out)
{
  std::string name, filename;
  in >> name >> filename;

  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  std::ifstream file(filename);
  if (!file.is_open())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  Word w;
  while (file >> w)
  {
    it->second[w.text]++;
  }
}

void filonova::remove(DictionarySet &dicts, std::istream &in, std::ostream &out)
{
  std::string name;
  Word w;

  in >> name >> w;

  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    out << "<WRONG DICT>\n";
    return;
  }

  auto wIt = it->second.find(w.text);
  if (wIt == it->second.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  it->second.erase(wIt);
}

void filonova::clear(DictionarySet &dicts, std::istream &in, std::ostream &out)
{
  std::string name;
  in >> name;

  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    out << "<WRONG DICT>\n";
    return;
  }

  if (it->second.empty())
  {
    out << "<EMPTY>\n";
    return;
  }

  it->second.clear();
}

void filonova::contains(DictionarySet &dicts, std::istream &in, std::ostream &out)
{
  std::string name;
  Word w;
  in >> name >> w;

  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    out << "<WRONG DICT>\n";
    return;
  }

  bool found = it->second.count(w.text);
  out << (found ? "<YES>\n" : "<NO>\n");
}

void filonova::merge(DictionarySet &dicts, std::istream &in, std::ostream &out)
{
  std::string newDict, dict1, dict2;
  if (!(in >> newDict >> dict1 >> dict2))
  {
    out << "<INVALID COMMAND>\n";
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }

  auto it1 = dicts.find(dict1);
  auto it2 = dicts.find(dict2);

  if (!isValidName(newDict) || it1 == dicts.end() || it2 == dicts.end())
  {
    out << "<WRONG DICT>\n";
    return;
  }

  Dictionary &dictNew = dicts[newDict];
  dictNew.clear();

  std::copy(
    it1->second.begin(),
    it1->second.end(),
    std::inserter(dictNew, dictNew.end()));

  std::for_each(
    it2->second.begin(),
    it2->second.end(),
    MergeDictEntry(dictNew));
}

void filonova::print(DictionarySet &dicts, std::istream &in, std::ostream &out)
{
  std::string name;
  in >> name;

  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    out << "<WRONG DICT>\n";
    return;
  }

  if (it->second.empty())
  {
    out << "<EMPTY>\n";
    return;
  }

  std::vector< std::pair< std::string, size_t > > words(it->second.begin(), it->second.end());
  std::sort(
    words.begin(),
    words.end(),
    CompareByFrequency(true));

  for (const auto &current : words)
  {
    out << Word{current.first} << ": " << current.second << '\n';
  }
}

void filonova::count(DictionarySet &dicts, std::istream &in, std::ostream &out)
{
  std::string name;
  Word w;
  in >> name >> w;

  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    out << "<WRONG DICT>\n";
    return;
  }

  auto wIt = it->second.find(w.text);
  if (wIt == it->second.end())
  {
    out << "<NOT FOUND>\n";
  }
  else
  {
    out << wIt->second << "\n";
  }
}

void filonova::top(DictionarySet &dicts, std::istream &in, std::ostream &out)
{
  std::string name;
  int count;
  in >> name >> count;

  if (in.fail() || count <= 0)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    out << "<WRONG DICT>\n";
    return;
  }

  if (it->second.empty())
  {
    out << "<EMPTY>\n";
    return;
  }

  std::vector< std::pair< std::string, size_t > > words(it->second.begin(), it->second.end());
  std::sort(
    words.begin(),
    words.end(),
    CompareByFrequency(true));

  size_t limit = std::min(static_cast< size_t >(count), words.size());
  for (size_t i = 0; i < limit; ++i)
  {
    out << Word{words[i].first} << ": " << words[i].second << "\n";
  }
}

void filonova::unique(DictionarySet &dicts, std::istream &in, std::ostream &out)
{
  std::string name;
  in >> name;

  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    out << "<WRONG DICT>\n";
    return;
  }

  out << it->second.size() << "\n";
}

void filonova::mostrare(DictionarySet &dicts, std::istream &in, std::ostream &out)
{
  std::string name;
  int count;
  in >> name >> count;

  if (in.fail() || count <= 0)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    out << "<WRONG DICT>\n";
    return;
  }

  const auto &dict = it->second;
  if (dict.empty())
  {
    out << "<EMPTY>\n";
    return;
  }

  std::vector< std::pair< std::string, size_t > > words(dict.begin(), dict.end());
  std::sort(
    words.begin(),
    words.end(),
    CompareByFrequency(false));

  size_t limit = std::min(static_cast< size_t >(count), words.size());
  for (size_t i = 0; i < limit; ++i)
  {
    out << Word{words[i].first} << ": " << words[i].second << "\n";
  }
}

void filonova::intersectDictionary(DictionarySet &dicts, std::istream &in, std::ostream &out)
{
  std::string newDict, dict1, dict2;
  if (!(in >> newDict >> dict1 >> dict2))
  {
    out << "<INVALID COMMAND>\n";
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }

  auto it1 = dicts.find(dict1);
  auto it2 = dicts.find(dict2);
  if (!isValidName(newDict) || it1 == dicts.end() || it2 == dicts.end())
  {
    out << "<WRONG DICT>\n";
    return;
  }

  Dictionary &dictNew = dicts[newDict];
  dictNew.clear();

  std::copy_if(
    it1->second.begin(),
    it1->second.end(),
    std::inserter(dictNew, dictNew.begin()),
    WordPresenceFilter(it2->second, true));
}

void filonova::excludeDictionary(DictionarySet &dicts, std::istream &in, std::ostream &out)
{
  std::string newDict, dict1, dict2;
  if (!(in >> newDict >> dict1 >> dict2))
  {
    out << "<INVALID COMMAND>\n";
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }

  auto it1 = dicts.find(dict1);
  auto it2 = dicts.find(dict2);
  if (!isValidName(newDict) || it1 == dicts.end() || it2 == dicts.end())
  {
    out << "<WRONG DICT>\n";
    return;
  }

  Dictionary &dictNew = dicts[newDict];
  dictNew.clear();

  std::copy_if(
    it1->second.begin(),
    it1->second.end(),
    std::inserter(dictNew, dictNew.begin()),
    WordPresenceFilter(it2->second, false));
}
