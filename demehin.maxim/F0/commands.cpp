#include "commands.hpp"
#include <functional>
#include <algorithm>

namespace
{
  using namespace demehin;

  struct TranslationPrinter
  {
    std::ostream& out;
    mutable bool isFirst = true;

    void operator()(const std::string& tr) const
    {
      if (!isFirst)
      {
        out << ", ";
      }
      out << tr;
      isFirst = false;
    }
  };

  struct DictPrinter
  {
    std::ostream& out;

    void operator()(const std::pair< std::string, list_t >& unit)
    {
      out << unit.first << ": ";
      std::for_each(unit.second.begin(), unit.second.end(), TranslationPrinter{ out });
      out << "\n";
    }
  };

  struct RuFinder
  {
    const std::string& target;
    list_t& res;

    void operator()(const std::pair< std::string, list_t >& unit)
    {
      if (std::find(unit.second.begin(), unit.second.end(), target) != unit.second.end())
      {
        res.push_back(unit.first);
      }
    }
  };

  struct NameReader
  {
    std::istream& in;
    std::string operator()()
    {
      std::string str;
      in >> str;
      return str;
    }
  };

  struct TranslationsModifier
  {
    list_t& translations;
    bool isAddOperation;

    void operator()(const std::string& word)
    {
      if (isAddOperation)
      {
        translations.push_back(word);
      }
      else
      {
        translations.remove(word);
      }
    }
  };

  void modifyTranslations(std::istream& in, dict_t& dicts, bool isAddOp)
  {
    std::string dict_name, eng_word;
    size_t translations_cnt;
    in >> dict_name >> eng_word >> translations_cnt;

    auto& dict = dicts.at(dict_name);
    auto& translations = dict.at(eng_word);
    list_t words;
    NameReader reader{ in };
    TranslationsModifier mod{ translations, isAddOp };

    std::generate_n(std::back_inserter(words), translations_cnt, reader);
    std::for_each(words.begin(), words.end(), mod);
  }

}

void demehin::printHelp(std::ostream& out)
{
  out << "Commands system:\n";
  out << "1. createdict < dictname > - create empty dictionary\n";
  out << "2. deletedict < dictname > - delete excisting dictionary\n";
  out << "3. printdict < dictname > - print dict content\n";
  out << "4. gettranslationeng < dictname > < eng > - print translations of word eng\n";
  out << "5. gettranslationru < dictname > < ru > - print translations of word ru\n";
  out << "6. deleteeng < dictname > < eng > - delete word eng from dictionary\n";
  out << "7. addeng < dictname > < eng > < N > < ru-1 > ... < ru-n > - add word eng with its N translations\n";
  out << "8. addru < dictname > < eng > < N > < ru-1 > ... < ru-n > - add N translations to eng\n";
  out << "9. deleteru < dictname > < eng > < N > < ru-1 > ... < ru-n > - delete N translations of eng\n";
  out << "10. union < newdict > < N > < dictname-1 > ... < dictname-n > - union of N dictionaries\n";
  out << "11. complement < newdict > < N > < dictname-1 > ... < dictname-n > - complemention of N dictionaries\n";
  out << "12. intersect < newdict > < N > < dictname-1 > ... < dictname-n > - intersection of N dictionaries\n";
}

void demehin::createDict(std::istream& in, dict_t& dicts)
{
  std::string name;
  in >> name;
  tree_t dict;
  if (dicts.find(name) == dicts.end())
  {
    dicts.insert(std::make_pair(name, dict));
  }
  else
  {
    throw std::logic_error("existent dictionary");
  }
}

void demehin::deleteDict(std::istream& in, dict_t& dicts)
{
  std::string dict_name;
  in >> dict_name;
  if (dicts.find(dict_name) == dicts.end())
  {
    throw std::logic_error("non-existent dictionary");
  }

  dicts.erase(dict_name);
}

void demehin::printDict(std::istream& in, std::ostream& out, const dict_t& dicts)
{
  std::string name;
  in >> name;
  auto dict = dicts.at(name);
  std::for_each(dict.begin(), dict.end(), DictPrinter{ out });
}

void demehin::getTranslationEng(std::istream& in, std::ostream& out, const dict_t& dicts)
{
  std::string dict_name, eng_word;
  in >> dict_name >> eng_word;
  auto translations = dicts.at(dict_name).at(eng_word);
  out << eng_word << ": ";
  std::for_each(translations.begin(), translations.end(), TranslationPrinter{ out });
  out << "\n";
}

void demehin::getTranslationRu(std::istream& in, std::ostream& out, const dict_t& dicts)
{
  std::string dict_name, ru_word;
  in >> dict_name >> ru_word;
  list_t res;
  auto dict = dicts.at(dict_name);
  std::for_each(dict.begin(), dict.end(), RuFinder{ ru_word, res });

  if (res.empty())
  {
    throw std::logic_error("no such russian word");
  }

  out << ru_word << ": ";
  std::for_each(res.begin(), res.end(), TranslationPrinter{ out });
  out << "\n";
}

void demehin::deleteEng(std::istream& in, dict_t& dicts)
{
  std::string dict_name, word;
  in >> dict_name >> word;
  auto& dict = dicts.at(dict_name);
  if (dict.erase(word) == false)
  {
    throw std::logic_error("no such english word");
  }
}

void demehin::addEng(std::istream& in, dict_t& dicts)
{
  std::string dict_name, eng_word;
  size_t cnt;
  in >> dict_name >> eng_word >> cnt;

  auto& dict = dicts.at(dict_name);
  if (dict.count(eng_word) > 0)
  {
    throw std::logic_error("word already exists");
  }

  list_t translations;
  NameReader reader{ in };
  std::generate_n(std::back_inserter(translations), cnt, reader);
  dict.insert(std::make_pair(eng_word, translations));
}

void demehin::deleteRu(std::istream& in, dict_t& dicts)
{
  modifyTranslations(in, dicts, false);
}

void demehin::addRu(std::istream& in, dict_t& dicts)
{
  modifyTranslations(in, dicts, true);
}
