#include "commands.hpp"
#include <algorithm>
#include <iterator>
#include <fstream>

void smirnov::createCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string name;
  in >> name;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (dicts.find(name) != dicts.end())
  {
    out << "The dictionary with name " << name << " already exists.\n";
    return;
  }
  dicts[name] = Dict();
  out << name << " is successfully created.\n";
}

void smirnov::addCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string dictName, word, translation;
  in >> dictName >> word >> translation;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    out << "The dictionary with name " << dictName << " doesn't exist.\n";
    return;
  }
  auto & dict = dictIt->second;
  auto wordIt = dict.find(word);
  if (wordIt != dict.end())
  {
    if (std::find(wordIt->second.begin(), wordIt->second.end(), translation) != wordIt->second.end())
    {
      out << "The word " << word << " already exists in " << dictName << "\n";
      return;
    }
    wordIt->second.push_back(translation);
  }
  else
  {
    dict[word] = {translation};
  }
}

void smirnov::translateCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string dictName, word;
  in >> dictName >> word;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    out << "The dictionary with name " << dictName << " doesn't exist.\n";
    return;
  }
  auto & dict = dictIt->second;
  auto wordIt = dict.find(word);
  if (wordIt == dict.end())
  {
    out << "The word " << word << " doesn't exist in " << dictName << "\n";
    return;
  }
  std::copy(wordIt->second.begin(), wordIt->second.end(), std::ostream_iterator< std::string >(out, " "));
  out << "\n";
}

void smirnov::removeCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string dictName, word;
  in >> dictName >> word;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    out << "The dictionary with name " << dictName << " doesn't exist.\n";
    return;
  }
  auto & dict = dictIt->second;
  auto wordIt = dict.find(word);
  if (wordIt == dict.end())
  {
    out << "The word " << word << " doesn't exist in " << dictName << "\n";
    return;
  }
  dict.erase(wordIt);
  out << "The word " << word << " successfully deleted from " << dictName << "\n";
}

void smirnov::printCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    out << "The dictionary with name " << dictName << " doesn't exist.\n";
    return;
  }
  const auto & dict = dictIt->second;
  if (dict.empty())
  {
    out << dictName << " is empty.\n";
    return;
  }
  for (const auto & pair : dict)
  {
    out << pair.first << " - ";
    std::copy(pair.second.begin(), pair.second.end(), std::ostream_iterator< std::string >(out, " "));
    out << "\n";
  }
}

void smirnov::saveCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string dictName, fileName;
  in >> dictName >> fileName;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (fileName.size() < 4 || fileName.substr(fileName.size() - 4) != ".txt")
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    out << "The dictionary with name " << dictName << " doesn't exist.\n";
    return;
  }
  std::ofstream file(fileName);
  if (!file)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  file << dictName << "\n";
  for (const auto & pair : dictIt->second)
  {
    file << pair.first << " - ";
    std::copy(pair.second.begin(), pair.second.end(), std::ostream_iterator< std::string >(file, " "));
    file << "\n";
  }
}

void smirnov::mergeCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string newName, dict1Name, dict2Name;
  in >> newName >> dict1Name >> dict2Name;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (dicts.find(newName) != dicts.end())
  {
    out << "The dictionary with name " << newName << " already exists.\n";
    return;
  }
  auto it1 = dicts.find(dict1Name);
  auto it2 = dicts.find(dict2Name);
  if (it1 == dicts.end() || it2 == dicts.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  Dict result = it1->second;
  for (const auto & pair : it2->second)
  {
    auto & translations = result[pair.first];
    for (const auto & tr : pair.second)
    {
      if (std::find(translations.begin(), translations.end(), tr) == translations.end())
      {
        translations.push_back(tr);
      }
    }
  }
  dicts[newName] = std::move(result);
  out << "Dictionary " << newName << " is successfully created\n";
}

void smirnov::deleteCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto it = dicts.find(dictName);
  if (it == dicts.end())
  {
    out << "The dictionary with name " << dictName << " doesn't exist.\n";
    return;
  }
  dicts.erase(it);
}

void smirnov::editCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string dictName, word, newTranslation;
  in >> dictName >> word >> newTranslation;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    out << "The dictionary with name " << dictName << " doesn't exist.\n";
    return;
  }
  auto & dict = dictIt->second;
  auto wordIt = dict.find(word);
  if (wordIt == dict.end())
  {
    out << "The word " << word << " doesn't exist in " << dictName << "\n";
    return;
  }
  wordIt->second.clear();
  wordIt->second.push_back(newTranslation);
}

void smirnov::renameCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string oldName, newName;
  in >> oldName >> newName;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto oldIt = dicts.find(oldName);
  if (oldIt == dicts.end())
  {
    out << "The dictionary with name " << oldName << " doesn't exist.\n";
    return;
  }
  if (dicts.find(newName) != dicts.end())
  {
    out << "The dictionary with name " << newName << " already exists.\n";
    return;
  }
  dicts[newName] = std::move(oldIt->second);
  dicts.erase(oldIt);
}

void smirnov::moveCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string fromDict, toDict, word;
  in >> fromDict >> toDict >> word;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto fromIt = dicts.find(fromDict);
  auto toIt = dicts.find(toDict);
  if (fromIt == dicts.end() || toIt == dicts.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto & from = fromIt->second;
  auto & to = toIt->second;
  auto wordIt = from.find(word);
  if (wordIt == from.end())
  {
    out << "The word " << word << " doesn't exist in " << fromDict << "\n";
    return;
  }
  if (to.find(word) != to.end())
  {
    out << "The word " << word << " already exists in " << toDict << "\n";
    return;
  }
  to[word] = std::move(wordIt->second);
  from.erase(wordIt);
  out << "The word " << word << " moved from " << fromDict << " to " << toDict << "\n";
}

void smirnov::copyCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string fromDict, toDict, word;
  in >> fromDict >> toDict >> word;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto fromIt = dicts.find(fromDict);
  auto toIt = dicts.find(toDict);
  if (fromIt == dicts.end() || toIt == dicts.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto & from = fromIt->second;
  auto & to = toIt->second;
  auto wordIt = from.find(word);
  if (wordIt == from.end())
  {
    out << "The word " << word << " doesn't exist in " << fromDict << "\n";
    return;
  }
  if (to.find(word) != to.end())
  {
    out << "The word " << word << " already exists in " << toDict << "\n";
    return;
  }
  to[word] = wordIt->second;
  out << "The word " << word << " copied from " << fromDict << " to " << toDict << "\n";
}

void smirnov::intersectCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string newName, dict1Name, dict2Name;
  in >> newName >> dict1Name >> dict2Name;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (dicts.find(newName) != dicts.end())
  {
    out << "The dictionary with name " << newName << " already exists.\n";
    return;
  }
  auto it1 = dicts.find(dict1Name);
  auto it2 = dicts.find(dict2Name);
  if (it1 == dicts.end() || it2 == dicts.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  Dict result;
  for (const auto & pair : it1->second)
  {
    if (it2->second.find(pair.first) != it2->second.end())
    {
      result[pair.first] = pair.second;
    }
  }
  dicts[newName] = std::move(result);
  out << "Dictionary " << newName << " is successfully created\n";
}

void smirnov::differenceCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string newName, dict1Name, dict2Name;
  in >> newName >> dict1Name >> dict2Name;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (dicts.find(newName) != dicts.end())
  {
    out << "The dictionary with name " << newName << " already exists.\n";
    return;
  }
  auto it1 = dicts.find(dict1Name);
  auto it2 = dicts.find(dict2Name);
  if (it1 == dicts.end() || it2 == dicts.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  Dict result;
  for (const auto & pair : it1->second)
  {
    if (it2->second.find(pair.first) == it2->second.end())
    {
      result[pair.first] = pair.second;
    }
  }
  dicts[newName] = std::move(result);
  out << "Dictionary " << newName << " is successfully created\n";
}

void smirnov::uniqueCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string newName, dict1Name, dict2Name;
  in >> newName >> dict1Name >> dict2Name;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (dicts.find(newName) != dicts.end())
  {
    out << "The dictionary with name " << newName << " already exists.\n";
    return;
  }
  auto it1 = dicts.find(dict1Name);
  auto it2 = dicts.find(dict2Name);
  if (it1 == dicts.end() || it2 == dicts.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  Dict result;
  for (const auto & pair : it1->second)
  {
    if (it2->second.find(pair.first) == it2->second.end())
    {
      result[pair.first] = pair.second;
    }
  }
  for (const auto & pair : it2->second)
  {
    if (it1->second.find(pair.first) == it1->second.end())
    {
      result[pair.first] = pair.second;
    }
  }
  dicts[newName] = std::move(result);
  out << "Dictionary " << newName << " is successfully created\n";
}

void smirnov::prefixCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string newName, dictName, prefix;
  in >> newName >> dictName >> prefix;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    out << "The dictionary with name " << dictName << " doesn't exist.\n";
    return;
  }
  if (dicts.find(newName) != dicts.end())
  {
    out << "The dictionary with name " << newName << " already exists.\n";
    return;
  }
  const auto & dict = dictIt->second;
  if (dict.empty())
  {
    out << dictName << " is empty.\n";
    return;
  }
  Dict result;
  for (const auto & pair : dict)
  {
    if (pair.first.substr(0, prefix.size()) == prefix)
    {
      result[pair.first] = pair.second;
    }
  }
  if (result.empty())
  {
    out << "There aren't any words in " << dictName << " with prefix " << prefix << ".\n";
    return;
  }
  dicts[newName] = std::move(result);
}

void smirnov::loadFile(Dicts & dicts, const std::string & filename)
{
  std::ifstream file(filename);
  if (!file)
  {
    throw std::runtime_error("Cannot open file\n");
  }
  std::string dictName;
  std::getline(file, dictName);
  Dict dict;
  std::string line;
  while (std::getline(file, line))
  {
    if (line.empty())
    {
      continue;
    }
    size_t dashPos = line.find(" - ");
    if (dashPos == std::string::npos)
    {
      continue;
    }
    std::string word = line.substr(0, dashPos);
    std::string translationsStr = line.substr(dashPos + 3);
    std::vector< std::string > translations;
    size_t start = 0;
    size_t end = translationsStr.find(' ');
    while (end != std::string::npos)
    {
      std::string translation = translationsStr.substr(start, end - start);
      if (!translation.empty())
      {
        translations.push_back(translation);
      }
      start = end + 1;
      end = translationsStr.find(' ', start);
    }
    std::string lastTranslation = translationsStr.substr(start);
    if (!lastTranslation.empty())
    {
      translations.push_back(lastTranslation);
    }
    dict[word] = translations;
  }
  dicts[dictName] = dict;
}

void smirnov::helpCommand(std::ostream & out)
{
  out << "Available commands:\n";
  out << "1) create <dict> - Create a new dictionary\n";
  out << "2) add <dict> <key> <translation> - Add word to dictionary\n";
  out << "3) translate <dict> <key> - Translate a word\n";
  out << "4) remove <dict> <key> - Remove a word\n";
  out << "5) print <dict> - Print dictionary contents\n";
  out << "6) save <dict> <filename> - Save dictionary to file\n";
  out << "7) merge <resdict> <dict1> <dict2> - Merge two dictionaries\n";
  out << "8) delete <dict> - Delete a dictionary\n";
  out << "9) edit <dict> <key> <newtranslation> - Edit word translation\n";
  out << "10) rename <dict> <newname> - Rename dictionary\n";
  out << "11) move <dict1> <dict2> <key> - Move word between dictionaries\n";
  out << "12) copy <dict1> <dict2> <key> - Copy word between dictionaries\n";
  out << "13) intersect <resdict> <dict1> <dict2> - Intersect two dictionaries\n";
  out << "14) difference <resdict> <dict1> <dict2> - Difference of dictionaries\n";
  out << "15) unique <resdict> <dict1> <dict2> - Unique words from dictionaries\n";
  out << "16) prefix <newdict> <dict> <prefix> - Words with given prefix\n";
}
