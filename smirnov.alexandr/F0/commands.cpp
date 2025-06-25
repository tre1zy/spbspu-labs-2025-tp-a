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
