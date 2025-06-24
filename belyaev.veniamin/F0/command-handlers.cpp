#include "command-handlers.hpp"

void belyaev::insertEntry(Dictionaries& data, std::istream& in, std::ostream& out)
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
  if (!isRuWordInDictionary(*currentDictionary, getItOfWordInDictByRu(*currentDictionary, russianWord)))
  {
    currentDictionary->dict[russianWord] = translation;
  }
  else
  {
    out << "<THIS ENTRY ALREADY EXISTS>\n";
  }
}

void belyaev::removeEntry(Dictionaries& data, std::istream& in, std::ostream& out)
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
  if (!isRuWordInDictionary(*currentDictionary, getItOfWordInDictByRu(*currentDictionary, russianWord)))
  {
    out << "<THIS ENTRY DOES NOT EXIST>\n";
    return;
  }
  currentDictionary->dict.erase(russianWord);
}

void belyaev::searchEntry(const Dictionaries& data, std::istream& in, std::ostream& out)
{
  std::string dictionaryName, russianWord;
  in >> dictionaryName >> russianWord;
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
  dictionaryIterator entry = getItOfWordInDictByRu(*currentDictionary, russianWord);
  if (!isRuWordInDictionary(*currentDictionary, entry))
  {
    out << "<THIS ENTRY DOES NOT EXIST>\n";
    return;
  }
  const std::pair<const std::string, std::string> entryPair = *entry;
  out << formPairString(entryPair) << '\n';
}

void belyaev::searchEntryByEnglish(const Dictionaries& data, std::istream& in, std::ostream& out)
{
  std::string dictionaryName, translation;
  in >> dictionaryName >> translation;
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
  dictionaryIterator entry = getItOfWordInDictByEn(*currentDictionary, translation);
  if (!isEnWordInDictionary(*currentDictionary, entry))
  {
    out << "<THIS ENTRY DOES NOT EXIST>\n";
  }
  const std::pair<const std::string, std::string> entryPair = *entry;
  out << formPairString(entryPair) << '\n';
}

void belyaev::printDict(const Dictionaries& data, std::istream& in, std::ostream& out)
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

belyaev::commandMap belyaev::mapCommandHandlers(Dictionaries& data)
{
  using namespace std::placeholders;
  commandMap cmds;
  cmds["INSERT"] = std::bind(insertEntry, std::ref(data), _1, _2);
  cmds["REMOVE"] = std::bind(removeEntry, std::ref(data), _1, _2);
  cmds["SEARCH"] = std::bind(searchEntry, std::cref(data), _1, _2);
  cmds["SEARCH_BY_ENGLISH"] = std::bind(searchEntryByEnglish, std::cref(data), _1, _2);
  cmds["PRINT"] = std::bind(printDict, std::cref(data), _1, _2);
  /*
  cmds["SEARCH_CONTAINS"] = std::bind(area, std::cref(data), _1, _2);
  cmds["SEARCH_CONTAINS_ENGLISH"] = std::bind(area, std::cref(data), _1, _2);
  cmds["PRINT_ALL"] = std::bind(area, std::cref(data), _1, _2);
  cmds["CLEAR"] = std::bind(area, std::ref(data), _1, _2);
  cmds["COPY"] = std::bind(area, std::ref(data), _1, _2);
  cmds["RENAME"] = std::bind(area, std::ref(data), _1, _2);
  cmds["MERGE"] = std::bind(area, std::ref(data), _1, _2);
  cmds["INTERSECT"] = std::bind(area, std::ref(data), _1, _2);
  cmds["DELETE_INTERSECTIONS"] = std::bind(area, std::ref(data), _1, _2);
  cmds["HELP"] = std::bind(area, _1, _2);
  */
  return cmds;
}
