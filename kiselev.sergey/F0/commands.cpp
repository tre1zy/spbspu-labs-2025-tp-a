#include "commands.hpp"
#include <ostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <vector>

namespace
{
  using osIt = std::ostream_iterator< std::string >;
  const std::string& extractDict(const kiselev::Dicts::value_type& val)
  {
    return val.first;
  }
  struct WordPrinter
  {
    std::ostream& out;
    void operator()(const std::string& word)
    {
      out << " " << word;
    }
  };
  struct DictPrinter
  {
    std::ostream& out;

    void operator()(const kiselev::Dict::value_type& val) const
    {
      out << val.first;
      if (!val.second.empty())
      {
        std::transform(val.second.begin(), val.second.end(), osIt(out), WordPrinter{ out });
      }
      out << "\n";
    }
  };
  struct TranslationChecker
  {
    const std::string& str;
    bool operator()(const kiselev::Dict::value_type& val)
    {
      return std::find(val.second.begin(), val.second.end(), str) != val.second.end();
    }
  };

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
  std::transform(dicts.begin(), dicts.end(), osIt(out, "\n"), extractDict);
}

void kiselev::doPrintDict(std::istream& in, std::ostream& out, const Dicts& dicts)
{
  std::string nameDict;
  in >> nameDict;
  auto dictIt = dicts.find(nameDict);
  if (dictIt == dicts.cend())
  {
    out << "<DICTIONARY NOT FOUND>\n";
    return;
  }
  out << nameDict << '\n';
  std::transform(dictIt->second.begin(), dictIt->second.end(), osIt(out), DictPrinter{out});
}

void kiselev::doTranslateWord(std::istream& in, std::ostream& out, const Dicts& dicts)
{
  std::string nameDict;
  std::string word;
  in >> nameDict >> word;
  auto dictIt = dicts.find(nameDict);
  if (dictIt == dicts.cend())
  {
    out << "<DICTIONARY NOT FOUND>\n";
    return;
  }
  const Dict dict = dictIt->second;
  auto engIt = dict.find(word);
  if (engIt != dict.cend())
  {
    out << *engIt->second.begin();
    std::transform(std::next(engIt->second.begin()), engIt->second.end(), osIt(out), WordPrinter{ out });
    out << "\n";
    return;
  }
  std::vector< std::string > translations;
  TranslationChecker check{ word };
  std::copy_if(dict.begin(), dict.end(), std::back_inserter(translations), check);
  if (!translations.empty())
  {
    out << *translations.begin();
    std::transform(std::next(engIt->second.begin()), engIt->second.end(), osIt(out), WordPrinter{ out });
    out << "\n";
    return;
  }
  out << "<WORD NOT FOUND>\n";
}
