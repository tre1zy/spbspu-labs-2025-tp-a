#include "command-handlers.hpp"
#include <iterator>
#include <algorithm>
#include <numeric>

void belyaev::insertEntryCmd(Dictionaries& data, std::istream& in, std::ostream& out)
{
  std::string dictionaryName, russianWord, translation;
  in >> dictionaryName >> russianWord >> translation;
  if (in.fail())
  {
    throw std::logic_error("Input failed in INSERT.");
  }

  Dictionary* currentDictionary = searchDictByName(data, dictionaryName);
  if (currentDictionary == nullptr)
  {
    data.dicts[dictionaryName] = Dictionary();
    currentDictionary = &data.dicts[dictionaryName];
  }
  if (!insertEntry(*currentDictionary, russianWord, translation))
  {
    out << "<THIS ENTRY ALREADY EXISTS>\n";
  }
}

void belyaev::removeEntryCmd(Dictionaries& data, std::istream& in, std::ostream& out)
{
  std::string dictionaryName, russianWord;
  in >> dictionaryName >> russianWord;
  if (in.fail())
  {
    throw std::logic_error("Input failed in REMOVE.");
  }

  Dictionary* currentDictionary = searchDictByName(data, dictionaryName);
  if (currentDictionary == nullptr)
  {
    out << "<THIS DICTIONARY DOES NOT EXIST>\n";
    return;
  }
  if (!removeEntry(*currentDictionary, russianWord))
  {
    out << "<THIS ENTRY DOES NOT EXIST>\n";
    return;
  }
}

void belyaev::searchEntryCmd(const Dictionaries& data, std::istream& in, std::ostream& out, std::string ruOrEng)
{
  using namespace std::placeholders;

  std::string dictionaryName, word;
  in >> dictionaryName >> word;
  if (in.fail())
  {
    throw std::logic_error("Input failed in REMOVE.");
  }

  const Dictionary* currentDictionary = searchDictByName(data, dictionaryName);
  if (currentDictionary == nullptr)
  {
    out << "<THIS DICTIONARY DOES NOT EXIST>\n";
    return;
  }
  std::map<std::string, std::function<dictionaryIterator(const Dictionary&, const std::string&)>> commandRuOrEng;
  commandRuOrEng["RU"] = std::bind(getItOfWordInDictByRu, _1, _2);
  commandRuOrEng["ENG"] = std::bind(getItOfWordInDictByEn, _1, _2);
  dictionaryIterator entry;
  try
  {
    entry = commandRuOrEng.at(ruOrEng)(*currentDictionary, word);
    if (!isRuWordInDictionary(*currentDictionary, entry))
    {
      out << "<THIS ENTRY DOES NOT EXIST>\n";
      return;
    }
    out << formPairString(*entry) << '\n';
  }
  catch (std::out_of_range& e)
  {
    throw std::logic_error("Subcommand in SEARCH out of confines");
  }
}

void belyaev::searchContainsCmd(const Dictionaries& data, std::istream& in, std::ostream& out, std::string ruOrEng)
{
  using namespace std::placeholders;

  std::string dictionaryName, word;
  in >> dictionaryName >> word;
  if (in.fail())
  {
    throw std::logic_error("Input failed in REMOVE.");
  }

  const Dictionary* currentDictionary = searchDictByName(data, dictionaryName);
  if (currentDictionary == nullptr)
  {
    out << "<THIS DICTIONARY DOES NOT EXIST>\n";
    return;
  }

  std::map<std::string, std::function<bool(const entryPair&)>> commandRuOrEng;
  commandRuOrEng["RU"] = std::bind(pairContainsRuChars, _1, word);
  commandRuOrEng["ENG"] = std::bind(pairContainsEnChars, _1, word);

  std::vector<nonConstEntryPair> dictCopy;
  std::copy(currentDictionary->dict.begin(), currentDictionary->dict.end(), std::back_inserter(dictCopy));

  try
  {
    using ostreamItStr = std::ostream_iterator<std::string>;
    auto it = std::stable_partition(dictCopy.begin(), dictCopy.end(), commandRuOrEng[ruOrEng]);
    std::transform(dictCopy.begin(), it, ostreamItStr{out, "\n"}, formPairString);
  }
  catch (const std::out_of_range& e)
  {
    throw std::logic_error("Subcommand in SEARCH_CONTAINS out of confines");
  }
}

void belyaev::printDictCmd(const Dictionaries& data, std::istream& in, std::ostream& out)
{
  std::string dictionaryName;
  in >> dictionaryName;
  if (in.fail())
  {
    throw std::logic_error("Input failed in PRINT.");
  }

  const Dictionary* currentDictionary = searchDictByName(data, dictionaryName);
  if (currentDictionary == nullptr)
  {
    out << "<THIS DICTIONARY DOES NOT EXIST>\n";
  }
  else
  {
    out << *currentDictionary;
  }
}

void belyaev::printAllDictCmd(const Dictionaries& data, std::ostream& out)
{
  if (data.dicts.size() == 0)
  {
    out << "<NO DICTIONARIES>\n";
    return;
  }
  auto printAllHelperBind = std::bind(printAllHelper, std::placeholders::_1, std::ref(out));
  std::transform(data.dicts.begin(), data.dicts.end(), std::ostream_iterator<Dictionary>{out, "\n"}, printAllHelperBind);
}

void belyaev::clearDictionaryCmd(Dictionaries& data, std::istream& in, std::ostream& out)
{
  std::string dictionaryName;
  in >> dictionaryName;
  if (in.fail())
  {
    throw std::logic_error("Input failed in CLEAR.");
  }

  Dictionary* currentDictionary = searchDictByName(data, dictionaryName);
  if (currentDictionary == nullptr)
  {
    out << "<THIS DICTIONARY DOES NOT EXIST>\n";
    return;
  }
  data.dicts.erase(dictionaryName);
}

void belyaev::clearAllDictionariesCmd(Dictionaries& data)
{
  data.dicts.clear();
}

void belyaev::copyDictionaryCmd(Dictionaries& data, std::istream& in, std::ostream& out)
{
  std::string dictionaryOld, dictionaryNew;
  in >> dictionaryOld >> dictionaryNew;
  if (in.fail())
  {
    throw std::logic_error("Input failed in COPY.");
  }
  if (dictionaryOld == dictionaryNew)
  {
    out << "<DICTIONARY_OLD EQUALS DICTIONARY_NEW>\n";
    return;
  }

  Dictionary* currentDict = searchDictByName(data, dictionaryOld);
  if (currentDict == nullptr)
  {
    out << "<DICTIONARY_OLD DOES NOT EXIST>\n";
    return;
  }
  Dictionary* newDict = searchDictByName(data, dictionaryNew);
  if (newDict != nullptr)
  {
    out << "<DICTIONARY_NEW ALREADY EXISTS>\n";
    return;
  }
  newDict = new Dictionary;
  std::copy(currentDict->dict.begin(), currentDict->dict.end(), std::inserter(newDict->dict, newDict->dict.begin()));
  data.dicts[dictionaryNew] = *newDict;
  delete newDict;
}

void belyaev::renameDictionaryCmd(Dictionaries& data, std::istream& in, std::ostream& out)
{
  std::string dictionaryOld, dictionaryNew;
  in >> dictionaryOld >> dictionaryNew;
  if (in.fail())
  {
    throw std::logic_error("Input failed in RENAME.");
  }
  if (dictionaryOld == dictionaryNew)
  {
    out << "<DICTIONARY_OLD EQUALS DICTIONARY_NEW>\n";
    return;
  }

  Dictionary* currentDict = searchDictByName(data, dictionaryOld);
  if (currentDict == nullptr)
  {
    out << "<DICTIONARY_OLD DOES NOT EXIST>\n";
    return;
  }
  Dictionary* newDict = searchDictByName(data, dictionaryNew);
  if (newDict != nullptr)
  {
    out << "<DICTIONARY_NEW ALREADY EXISTS>\n";
    return;
  }

  std::map<std::string, std::string> mapCopy = currentDict->dict;
  data.dicts.erase(dictionaryOld);
  newDict = new Dictionary;
  newDict->dict = mapCopy;
  data.dicts[dictionaryNew] = *newDict;
  delete newDict;
}

void belyaev::mergeDictionariesCmd(Dictionaries& data, std::istream& in, std::ostream& out)
{
  using namespace std::placeholders;

  size_t dictionariesAmount;
  std::string dictionaryNew;

  in >> dictionariesAmount >> dictionaryNew;
  using istreamItStr = std::istream_iterator<std::string>;
  std::vector<std::string> dictNamesToMerge;
  std::copy_n(istreamItStr{in}, dictionariesAmount, std::back_inserter(dictNamesToMerge));
  if (in.fail())
  {
    throw std::logic_error("Input failed in MERGE.");
  }

  areAllNamesInDicts nameDictsTraversal(data, dictNamesToMerge);
  if (!nameDictsTraversal())
  {
    out << "<ONE OF THE DICTIONARIES DOESN'T EXIST>";
    return;
  }

  Dictionary newDict;
  mergeDictsIterator merge(data, newDict);
  std::copy(dictNamesToMerge.begin(), dictNamesToMerge.end(), merge);
  data.dicts[dictionaryNew] = newDict;
}

void belyaev::intersectDictionariesCmd(Dictionaries& data, std::istream& in, std::ostream& out)
{
  using namespace std::placeholders;

  size_t dictionariesAmount;
  std::string dictionaryNew;

  in >> dictionariesAmount >> dictionaryNew;
  using istreamItStr = std::istream_iterator<std::string>;
  std::vector<std::string> dictNamesToIntersect;
  std::copy_n(istreamItStr{in}, dictionariesAmount, std::back_inserter(dictNamesToIntersect));
  if (in.fail())
  {
    throw std::logic_error("Input failed in INTERSECT.");
  }

  areAllNamesInDicts nameDictsTraversal(data, dictNamesToIntersect);
  if (!nameDictsTraversal())
  {
    out << "<ONE OF THE DICTIONARIES DOESN'T EXIST>\n";
    return;
  }

  Dictionary newDict;
  intersectDictsIterator intersect(data, newDict, dictNamesToIntersect);
  std::copy(dictNamesToIntersect.begin(), dictNamesToIntersect.end(), intersect);
  data.dicts[dictionaryNew] = newDict;
}

void belyaev::deleteIntersectionsCmd(Dictionaries& data, std::istream& in, std::ostream& out)
{
  size_t dictionariesAmount;
  std::string dictionaryNew;

  in >> dictionariesAmount >> dictionaryNew;
  using istreamItStr = std::istream_iterator<std::string>;
  std::vector<std::string> dictNamesToProcess;
  std::copy_n(istreamItStr{in}, dictionariesAmount, std::back_inserter(dictNamesToProcess));
  if (in.fail())
  {
    throw std::logic_error("Input failed in DELETE_INTERSECTIONS.");
  }

  areAllNamesInDicts nameDictsTraversal(data, dictNamesToProcess);
  if (!nameDictsTraversal())
  {
    out << "<ONE OF THE DICTIONARIES DOES NOT EXIST>\n";
    return;
  }

  if (!dictNamesToProcess.empty())
  {
    std::string firstDictName = dictNamesToProcess[0];
    Dictionary* firstDict = searchDictByName(data, firstDictName);
    if (firstDict == nullptr)
    {
      out << "<ONE OF THE DICTIONARIES DOES NOT EXIST>\n";
      return;
    }

    std::vector<std::string> otherDictNames(dictNamesToProcess.begin() + 1, dictNamesToProcess.end());

    removeIntersectionsHelper remover(data, dictNamesToProcess);
    remover(firstDictName);

    data.dicts[dictionaryNew] = *firstDict;
  }
}

void belyaev::helpCmd(std::ostream& out)
{
  out << "1. INSERT <dictionary> <russian_word> <translation>\n"
      << "Добавляет запись с переводом в словарь. Если словаря \"dictionary\" не существует, то такой создается."
      << "Если в словаре такая запись уже существует, то вывести сообщение <THIS ENTRY ALREADY EXISTS>\n"
      << "2. REMOVE <dictionary> <russian_word> <translation>\n"
      << "Удаляет из словаря запись с переводом слова. Если в словаре таких не существует, то вывести сообщение"
      << "<THIS ENTRY DOES NOT EXIST>. Если словаря не существует, вывести <THIS DICTIONARY DOES NOT EXIST>.\n"
      << "3. SEARCH <dictionary> <russian_word>\n"
      << "Ищет в словаре записи с переводом слова и выводит их. Если поиск неудачен, то вывести сообщение"
      << "<THIS ENTRY DOES NOT EXIST>.\n"
      << "4. SEARCH_BY_ENGLISH <dictionary> <english_word>\n"
      << "Ищет в словаре записи, где переводом является указанное слово, и выводит их. Если поиск неудачен,"
      << "то вывести сообщение <THIS ENTRY DOES NOT EXIST>.\n"
      << "5. SEARCH_СONTAINS <dictionary> <russian_characters>\n"
      << "Ищет в словаре записи, где в русском слове содержится указанный порядок символов. Если поиск неудачен,"
      << "то вывести сообщение <NO ENTRIES>.\n"
      << "6. SEARCH_CONTAINS_ENGLISH <dictionary> <english_characters>\n"
      << "Ищет в словаре записи, где в переводе содержится указанный порядок символов. Если поиск неудачен,"
      << "то вывести сообщение <NO ENTRIES>.\n"
      << "7. PRINT <dictionary>\n"
      << "Выводит все записи из словаря. Если словаря не существует, то вывести сообщение"
      << "<THIS DICTIONARY DOES NOT EXIST>.\n"
      << "8. PRINT_ALL\n"
      << "Выводит все записи в системе.\n"
      << "9. CLEAR <dictionary>\n"
      << "Очищает все записи в словаре, как следствие, удаляя его. Если словаря не существует, то вывести сообщение"
      << "<THIS DICTIONARY DOES NOT EXIST>.\n"
      << "10. CLEAR_ALL\n"
      << "Очищает все записи в системе.\n"
      << "11. COPY <dictionary_old> <dictionary_new>\n"
      << "Копирует словарь \"dictionary_old\", присваивая новому имя \"dictionary_new\". Если словаря в первом поле"
      << "не существует, то вывести сообщение <THIS DICTIONARY DOES NOT EXIST>.\n"
      << "12. RENAME <dictionary_old> <dictionary_new>\n"
      << "Переименовывает словарь \"dictionary_old\", присваивая новому имя \"dictionary_new\". Если словаря в первом"
      << "поле не существует, то вывести сообщение <THIS DICTIONARY DOES NOT EXIST>.\n"
      << "13. MERGE <N> <dictionary_new> <dictionary1> ... <dictionaryN>\n"
      << "N - натуральное число больше 1. Команда совмещает все записи в словарях с 1 по N в новом словаре"
      << "\"dictionary_new\". Если одного из словарей не существует, то вывести сообщение"
      << "<ONE OF THE DICTIONARIES DOES NOT EXIST>.\n"
      << "14. INTERSECT <N> <dictionary_new> <dictionary1> ... <dictionaryN>\n"
      << "N - натуральное число больше 1. Сохраняет все пересечения словарей с 1 по N в новый словарь"
      << "\"dictionary_new\". Если одного из словарей не существует, то вывести сообщение"
      << "<ONE OF THE DICTIONARIES DOES NOT EXIST>.\n"
      << "15. DELETE_INTERSECTIONS <N> <dictionary_new> <dictionary1> ... <dictionaryN>\n"
      << "N - натуральное число больше 1. Удаляет все пересечения в первом словаре с остальными, с 2 по N."
      << "Если одного из словарей не существует, то вывести сообщение <ONE OF THE DICTIONARIES DOES NOT EXIST>.\n"
      << "16. HELP\n"
      << "Вывод всего списка команд со справкой.\n";
}

belyaev::commandMap belyaev::mapCommandHandlers(Dictionaries& data)
{
  using namespace std::placeholders;
  commandMap cmds;
  cmds["INSERT"] = std::bind(insertEntryCmd, std::ref(data), _1, _2);
  cmds["REMOVE"] = std::bind(removeEntryCmd, std::ref(data), _1, _2);
  cmds["SEARCH"] = std::bind(searchEntryCmd, std::cref(data), _1, _2, "RU");
  cmds["SEARCH_BY_ENGLISH"] = std::bind(searchEntryCmd, std::cref(data), _1, _2, "ENG");
  cmds["SEARCH_CONTAINS"] = std::bind(searchContainsCmd, std::cref(data), _1, _2, "RU");
  cmds["SEARCH_CONTAINS_ENGLISH"] = std::bind(searchContainsCmd, std::cref(data), _1, _2, "ENG");
  cmds["PRINT"] = std::bind(printDictCmd, std::cref(data), _1, _2);
  cmds["PRINT_ALL"] = std::bind(printAllDictCmd, std::cref(data), _2);
  cmds["CLEAR"] = std::bind(clearDictionaryCmd, std::ref(data), _1, _2);
  cmds["CLEAR_ALL"] = std::bind(clearAllDictionariesCmd, std::ref(data));
  cmds["COPY"] = std::bind(copyDictionaryCmd, std::ref(data), _1, _2);
  cmds["RENAME"] = std::bind(renameDictionaryCmd, std::ref(data), _1, _2);
  cmds["MERGE"] = std::bind(mergeDictionariesCmd, std::ref(data), _1, _2);
  cmds["INTERSECT"] = std::bind(intersectDictionariesCmd, std::ref(data), _1, _2);
  cmds["DELETE_INTERSECTIONS"] = std::bind(deleteIntersectionsCmd, std::ref(data), _1, _2);
  cmds["HELP"] = std::bind(helpCmd, _2);
  return cmds;
}
