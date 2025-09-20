#include "commands.hpp"
#include <algorithm>
#include <fstream>
#include <functional>
#include <iterator>
#include "Alphabet.hpp"
#include "ScopeGuard.hpp"

namespace
{
  std::string dictToDictName(const std::pair<const std::string, holodilov::Dictionary >& pair)
  {
    return pair.first;
  }

  std::list< std::string > dictToTranslations(const std::pair< std::string, holodilov::Dictionary >& pair, const std::string& enWord)
  {
    if (pair.second.dict.contains(enWord))
    {
      return pair.second.dict.at(enWord);
    }
    return std::list< std::string >();
  }

  bool checkIntersection(const std::pair< std::string, std::list< std::string > >& pair, const holodilov::Dictionary& dict)
  {
    return dict.dict.contains(pair.first);
  }

  std::list< std::string > translationsToStr(const std::list< std::string >& translations, std::list< std::string >& allTranslations)
  {
    allTranslations.insert(allTranslations.end(), translations.begin(), translations.end());
    return translations;
  }
}

void holodilov::createDict(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries)
{
  std::string name;
  in >> name;
  if (!in)
  {

    throw std::logic_error("Error: invalid command.\n");
  }
  std::string lang;
  in >> lang;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.\n");
  }

  if (dictionaries.contains(name))
  {
    throw std::logic_error("Error: dictionary with this name already exists.\n");
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
    throw std::logic_error("Error: invalid command.\n");
  }
  std::string fileName;
  in >> fileName;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.\n");
  }

  if (!dictionaries.contains(dictName))
  {
    throw std::logic_error("Error: dictionary not found.\n");
  }

  std::ofstream fout;
  fout.open(fileName);
  if (!fout.is_open())
  {
    throw std::logic_error("Error: unable to write dictionary into file.\n");
  }
  fout << dictionaries[dictName];
  if (!fout)
  {
    fout.close();
    throw std::logic_error("Error: unable to write dictionary into file.\n");
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
    throw std::logic_error("Error: invalid command.\n");
  }

  std::ifstream fis;
  fis.open(filename);
  if (!fis.is_open())
  {
    throw std::logic_error("Error: dictionary file not found.\n");
  }
  Dictionary dict;
  fis >> dict;
  if (!fis)
  {
    fis.close();
    throw std::logic_error("Error: dictionary file has invalid format.\n");
  }
  fis.close();

  if (dictionaries.contains(dict.name))
  {
    throw std::logic_error("Error: dictionary with this name already exists.\n");
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
    throw std::logic_error("Error: invalid command.\n");
  }

  if (!dictionaries.contains(dictName))
  {
    throw std::logic_error("Error: dictionary not found.\n");
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
    throw std::logic_error("Error: invalid command.\n");
  }

  if (!dictionaries.contains(dictName))
  {
    throw std::logic_error("Error: dictionary not found.\n");
  }
  dictionaries.erase(dictName);

  out << "Dictionary " << dictName << " was deleted.\n";
}

void holodilov::addWord(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.\n");
  }

  std::string englishWord;
  in >> englishWord;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.\n");
  }

  int amountTranslations = 0;
  in >> amountTranslations;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.\n");
  }

  if (!dictionaries.contains(dictName))
  {
    throw std::logic_error("Error: dictionary not found.\n");
  }
  dictionaries.at(dictName).dict[englishWord] = std::list< std::string >();

  using istreamIter = std::istream_iterator< std::string >;
  std::copy_n(istreamIter(in), amountTranslations, std::back_inserter(dictionaries[dictName].dict[englishWord]));
  if (!in)
  {
    throw std::logic_error("Error: invalid command.\n");
  }
  out << "Word " << englishWord << " was added to " << dictName << "\n";
}

void holodilov::deleteWord(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.\n");
  }

  std::string englishWord;
  in >> englishWord;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.\n");
  }

  if (!dictionaries.contains(dictName))
  {
    throw std::logic_error("Error: dictionary not found.\n");
  }

  if (!dictionaries.at(dictName).dict.contains(englishWord))
  {
    throw std::logic_error("Error: word not found in this dictionary.\n");
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
    throw std::logic_error("Error: invalid command.\n");
  }

  std::string englishWord;
  in >> englishWord;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.\n");
  }

  std::string englishWordNew;
  in >> englishWordNew;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.\n");
  }

  if (!dictionaries.contains(dictName))
  {
    throw std::logic_error("Error: dictionary not found.\n");
  }
  if (!dictionaries.at(dictName).dict.contains(englishWord))
  {
    throw std::logic_error("Error: word not found in this dictionary.\n");
  }

  std::list< std::string > translations = dictionaries.at(dictName).dict.at(englishWord);
  dictionaries.at(dictName).dict.erase(englishWord);
  dictionaries.at(dictName).dict[englishWordNew] = translations;
  out << "Word " << englishWord << " was updated to " << englishWordNew << "\n";
}

void holodilov::printDict(std::istream& in, std::ostream& out, const std::map< std::string, Dictionary >& dictionaries)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.\n");
  }

  if (!dictionaries.contains(dictName))
  {
    throw std::logic_error("Error: dictionary not found.\n");
  }
  out << dictionaries.at(dictName) << '\n';
}

void holodilov::findWord(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries)
{
  std::string englishWord;
  in >> englishWord;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.\n");
  }
  std::vector< std::list< std::string > > translLists(dictionaries.size());
  auto dictToTranslBound = std::bind(dictToTranslations, std::placeholders::_1, std::cref(englishWord));
  std::transform(dictionaries.begin(), dictionaries.end(), translLists.begin(), dictToTranslBound);

  std::list< std::string > listTranslations;
  auto translationsToStrBound = std::bind(translationsToStr, std::placeholders::_1, std::ref(listTranslations));
  std::transform(translLists.begin(), translLists.end(), translLists.begin(), translationsToStrBound);

  std::set< std::string > setTranslations(listTranslations.begin(), listTranslations.end());
  std::copy(setTranslations.begin(), setTranslations.end(), std::ostream_iterator< std::string >(out, "\n"));
}

void holodilov::merge(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries)
{
  std::string dictName1;
  in >> dictName1;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.\n");
  }

  std::string dictName2;
  in >> dictName2;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.\n");
  }

  std::string dictNameNew;
  in >> dictNameNew;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.\n");
  }

  std::string dictLangNew;
  in >> dictLangNew;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.\n");
  }

  if ((!dictionaries.contains(dictName1)) || (!dictionaries.contains(dictName2)))
  {
    throw std::logic_error("Error: dictionary not found.\n");
  }

  std::map< std::string, std::list< std::string > > mapNewDict = dictionaries.at(dictName1).dict;

  Dictionary& dict2 = dictionaries.at(dictName2);
  mapNewDict.insert(dict2.dict.begin(), dict2.dict.end());

  Dictionary dictNew(dictNameNew, dictLangNew);
  dictNew.dict = mapNewDict;
  dictionaries[dictNameNew] = dictNew;
  out << "Dictionaries " << dictName1 << " and " << dictName2 << " were merged to " << dictNameNew << "\n";
}

void holodilov::intersect(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries)
{
  std::string dictName1;
  in >> dictName1;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.\n");
  }

  std::string dictName2;
  in >> dictName2;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.\n");
  }

  std::string dictNameNew;
  in >> dictNameNew;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.\n");
  }

  std::string dictLangNew;
  in >> dictLangNew;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.\n");
  }

  if ((!dictionaries.contains(dictName1)) || (!dictionaries.contains(dictName2)))
  {
    throw std::logic_error("Error: dictionary not found.\n");
  }

  Dictionary& dict1 = dictionaries.at(dictName1);
  Dictionary& dict2 = dictionaries.at(dictName2);

  Dictionary dictNew(dictNameNew, dictLangNew);
  auto checkIntersectionBound = std::bind(checkIntersection, std::placeholders::_1, std::cref(dict2));

  auto inserterNewDict = std::inserter(dictNew.dict, dictNew.dict.end());
  std::copy_if(dict1.dict.begin(), dict1.dict.end(), inserterNewDict, checkIntersectionBound);

  dictionaries[dictNameNew] = dictNew;
  out << "Dictionaries " << dictName1 << " and " << dictName2 << " were intersected to " << dictNameNew << "\n";
}

void holodilov::exportAlphabet(std::istream& in, std::ostream& out, const std::map< std::string, Dictionary >& dictionaries)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.\n");
  }

  std::string filename;
  in >> filename;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.\n");
  }

  if (!dictionaries.contains(dictName))
  {
    throw std::logic_error("Error: dictionary not found.\n");
  }

  Alphabet alphabet;
  alphabet.load(dictionaries.at(dictName));

  std::ofstream fos;
  fos.open(filename);
  if (!fos.is_open())
  {
    throw std::logic_error("Error: unable to create alphabet file.\n");
  }
  fos << alphabet;
  if (!fos)
  {
    throw std::logic_error("Error: unable to create alphabet file.\n");
  }

  out << "Alphabet of " << dictName << " dictionary was exported into file " << filename << "\n";
}

void holodilov::checkAlphabet(std::istream& in, std::ostream& out, const std::map< std::string, Dictionary >& dictionaries)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.\n");
  }

  std::string filename;
  in >> filename;
  if (!in)
  {
    throw std::logic_error("Error: invalid command.\n");
  }

  if (!dictionaries.contains(dictName))
  {
    throw std::logic_error("Error: dictionary not found.\n");
  }

  Alphabet alphabetFromFile;

  std::ifstream fis;
  fis.open(filename);
  if (!fis.is_open())
  {
    throw std::logic_error("Error: alphabet file not found.\n");
  }
  fis >> alphabetFromFile;
  if (fis.fail() && !fis.eof())
  {
    throw std::logic_error("Error: unable to read alphabet file.\n");
  }

  Alphabet alphabetFromDict;
  alphabetFromDict.load(dictionaries.at(dictName));

  ScopeGuard scopeGuard(out);
  out << std::boolalpha;
  out << (alphabetFromFile == alphabetFromDict) << "\n";
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
