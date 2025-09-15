#include "commands.hpp"

#include <algorithm>
#include <fstream>
#include <functional>
#include <iterator>
#include <numeric>

#include "exceptions.hpp"


std::string dictToDictName(const std::pair<const std::string, holodilov::Dictionary >& pair)
{
  return pair.first;
}

std::list< std::string > dictToWordTranslations(const holodilov::Dictionary& dict, const std::string& englishWord)
{
  return dict.dict.at(englishWord);
}

std::list< std::string > translationsAccumulator(std::list< std::string >& translations, std::list< std::string > translation)
{
  translations.insert(translations.end(), translation.begin(), translation.end());
  return translations;
}
void holodilov::createDict(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries)
{
  std::string name;
  in >> name;
  if (!in)
  {
    throw InvalidCommandException();
  }
  std::string lang;
  in >> lang;
  if (!in)
  {
    throw InvalidCommandException();
  }

  if (dictionaries.contains(name))
  {
    throw DictionaryAlreadyExistsException();
  }

  dictionaries.emplace(name, Dictionary(name, lang));
  out << "Created dictionary:\n" << "name: " << name << "\nlang: " << lang << "\n";
}

void holodilov::exportDict(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    throw InvalidCommandException();
  }
  std::string fileName;
  in >> fileName;
  if (!in)
  {
    throw InvalidCommandException();
  }

  if (!dictionaries.contains(dictName))
  {
    throw DictionaryNotFoundException();
  }

  std::ofstream fout;
  fout.open(fileName);
  if (!fout.is_open())
  {
    throw DictionaryExportException();
  }
  fout << dictionaries[dictName];
  if (!fout)
  {
    fout.close();
    throw DictionaryExportException();
  }
  fout.close();

  out << "Dictionary " << dictName << " successfully exported into file " << fileName << "\n";
}

void holodilov::importDict(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries)
{
  std::string filename;
  in >> filename;
  if (!in)
  {
    throw InvalidCommandException();
  }

  std::ifstream fis;
  fis.open(filename);
  if (!fis.is_open())
  {
    throw DictionaryFileNotFoundException();
  }
  Dictionary dict;
  fis >> dict;
  if (!fis)
  {
    fis.close();
    throw InvalidDictionaryFileException();
  }
  fis.close();

  if (dictionaries.contains(dict.name))
  {
    throw DictionaryAlreadyExistsException();
  }

  dictionaries.emplace(dict.name, dict);
  out << "Dictionary " << dict.name << " was successfully imported.\n";
}

void holodilov::clearDict(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    throw InvalidCommandException();
  }

  if (!dictionaries.contains(dictName))
  {
    throw DictionaryNotFoundException();
  }
  dictionaries[dictName].dict.clear();

  out << "Dictionary " << dictName << " was cleared.\n";
}

void holodilov::deleteDict(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    throw InvalidCommandException();
  }

  if (!dictionaries.contains(dictName))
  {
    throw DictionaryNotFoundException();
  }
  dictionaries.erase(dictName);

  out << "Dictionary " << dictName << " was deleted.\n";
}

void holodilov::addWord(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries)
{

}

void holodilov::deleteWord(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    throw InvalidCommandException();
  }

  std::string englishWord;
  in >> englishWord;
  if (!in)
  {
    throw InvalidCommandException();
  }

  if (!dictionaries.contains(dictName))
  {
    throw DictionaryNotFoundException();
  }

  if (!dictionaries.at(dictName).dict.contains(englishWord))
  {
    throw WordNotFoundException();
  }

  dictionaries.at(dictName).dict.erase(englishWord);
  out << "Word " << englishWord << " was deleted from dictionary " << dictName << "\n";
}

void holodilov::updateWord(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    throw InvalidCommandException();
  }

  std::string englishWord;
  in >> englishWord;
  if (!in)
  {
    throw InvalidCommandException();
  }

  std::string englishWordNew;
  in >> englishWord;
  if (!in)
  {
    throw InvalidCommandException();
  }

  if (!dictionaries.contains(dictName))
  {
    throw DictionaryNotFoundException();
  }
  if (!dictionaries.at(dictName).dict.contains(englishWord))
  {
    throw WordNotFoundException();
  }

  std::list< std::string > translations = dictionaries.at(dictName).dict.at(englishWord);
  dictionaries.at(dictName).dict.erase(englishWord);
  dictionaries.at(dictName).dict[englishWordNew] = translations;
  out << "Word " << englishWord << "was updated to " << englishWordNew << "\n";
}

void holodilov::printDict(std::istream& in, std::ostream& out, const std::map< std::string, Dictionary >& dictionaries)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    throw InvalidCommandException();
  }

  if (!dictionaries.contains(dictName))
  {
    throw DictionaryNotFoundException();
  }
  out << dictionaries.at(dictName);
}

void holodilov::findWord(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries)
{
  std::string englishWord;
  in >> englishWord;
  if (!in)
  {
    throw InvalidCommandException();
  }

  std::vector< std::list< std::string > > vecTranslations(dictionaries.size());
  auto dictToWordTranslationsBound = std::bind(dictToWordTranslationsBound, std::placeholders::_1, std::cref(englishWord));
  std::transform(dictionaries.begin(), dictionaries.end(), vecTranslations.begin(), dictToWordTranslationsBound);

  std::list< std::string > translations = std::accumulate(vecTranslations.begin(), vecTranslations.end(), std::list< std::string >(), translationsAccumulator);

  using ostreamIter = std::ostream_iterator< std::string >;
  std::copy(translations.begin(), translations.end(), ostreamIter(out, "\n"));
}

void holodilov::merge(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries)
{

}

void holodilov::intersect(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries)
{

}

void holodilov::alphabet(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries)
{

}

void holodilov::printDictNames(std::ostream& out, const std::map< std::string, Dictionary >& dictionaries)
{
  if (dictionaries.empty())
  {
    out << "There are no dictionaries yet.\n";
    return;
  }

  using ostreamIter = std::ostream_iterator< std::string >;

  std::vector< std::string > dictNames(dictionaries.size());
  std::transform(dictionaries.begin(), dictionaries.end(), dictNames.begin(), dictToDictName);
  std::copy(dictNames.begin(), dictNames.end(), ostreamIter(out, "\n"));
}

