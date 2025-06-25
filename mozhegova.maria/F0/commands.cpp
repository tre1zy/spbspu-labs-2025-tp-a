#include "commands.hpp"
#include <fstream>
#include <algorithm>
#include <string>
#include <functional>
#include <numeric>
#include <iomanip>

namespace
{
  using VectorWordPos = std::vector< std::pair< size_t, size_t > >;
  bool cmpMaxWordLen(const std::pair< std::string, VectorWordPos > & a, const std::pair< std::string, VectorWordPos > & b)
  {
    return a.first.size() < b.first.size();
  }

  struct PrintWordPos
  {
    std::ostream & out;
    void operator()(const std::pair< size_t, size_t > & pos) const
    {
      out << " (" << pos.first << ", " << pos.second << ')';
    }
  };

  struct PrintWords
  {
    std::ostream & out;
    size_t width;
    void operator()(const std::pair< std::string, VectorWordPos > & word) const
    {
      out << std::left << std::setw(width) << word.first;
      std::for_each(word.second.cbegin(), word.second.cend(), PrintWordPos{out});
      out << '\n';
    }
  };

  std::pair< mozhegova::WordPos, std::string > createWordPair(const mozhegova::WordPos & pos, const std::string & word)
  {
    return std::make_pair(pos, word);
  }

  struct Accumulator
  {
    std::string result;
    std::pair< mozhegova::WordPos, std::string > prev;
    bool first = true;
    Accumulator(const std::string & initial):
      result(initial)
    {}
  };

  Accumulator accumulateWords(Accumulator acc, const std::pair< mozhegova::WordPos, std::string > & current)
  {
    if (acc.first)
    {
      acc.result += current.second;
      acc.first = false;
    }
    else
    {
      if (acc.prev.first.first != current.first.first)
      {
        acc.result += '\n';
      }
      else
      {
        acc.result += ' ';
      }
      acc.result += current.second;
    }
    acc.prev = current;
    return acc;
  }

  std::string reconstructText(const mozhegova::Text & text)
  {
    std::vector< std::pair< mozhegova::WordPos, std::string > > sortedWords;
    for (const auto & wordEntry : text)
    {
      auto b = wordEntry.second.begin();
      auto e = wordEntry.second.end();
      using namespace std::placeholders;
      std::transform(b, e, std::back_inserter(sortedWords), std::bind(createWordPair, _1, wordEntry.first));
    }
    std::sort(sortedWords.begin(), sortedWords.end());
    if (sortedWords.empty())
    {
      return "";
    }
    Accumulator initialValue("");
    Accumulator result = std::accumulate(sortedWords.begin(), sortedWords.end(), initialValue, accumulateWords);
    return result.result;
  }

  struct PrintText
  {
    std::ostream & out;
    void operator()(const std::pair< std::string, mozhegova::Text > & text) const
    {
      out << text.first << ' ' << text.second.size() << '\n';
      out << reconstructText(text.second);
    }
  };

  bool cmpMaxLineNumWord(const std::pair< size_t, size_t > & a, const std::pair< size_t, size_t > & b)
  {
    return a.first < b.first;
  }

  size_t getMaxLineNumWord(const std::pair< std::string, VectorWordPos > & word)
  {
    VectorWordPos xrefs = word.second;
    if (xrefs.empty())
    {
      return 0;
    }
    auto maxNumIt = std::max_element(xrefs.cbegin(), xrefs.cend(), cmpMaxLineNumWord);
    return maxNumIt->first;
  }

  size_t getMaxLineNum(const mozhegova::Text & text)
  {
    std::vector< size_t > lineNum;
    std::transform(text.cbegin(), text.cend(), std::back_inserter(lineNum), getMaxLineNumWord);
    return *std::max_element(lineNum.cbegin(), lineNum.cend());
  }

  bool cmpBetween(const mozhegova::WordPos & pos, size_t begin, size_t end)
  {
    return pos.first >= begin && pos.first < end;
  }

  mozhegova::Text extractSubstring(const mozhegova::Text & text, size_t begin, size_t end)
  {
    mozhegova::Text result;
    for (const auto & wordEntry : text)
    {
      VectorWordPos newXrefs;
      auto b = wordEntry.second.begin();
      auto e = wordEntry.second.end();
      using namespace std::placeholders;
      std::copy_if(b, e, std::back_inserter(newXrefs), std::bind(cmpBetween, _1, begin, end));
      if (!newXrefs.empty())
      {
        result[wordEntry.first] = newXrefs;
      }
    }
    return result;
  }

  mozhegova::WordPos downLenNum(const mozhegova::WordPos & pos, size_t begin, size_t end)
  {
    if (pos.first >= end)
    {
      return {pos.first - (end - begin), pos.second};
    }
    return pos;
  }

  void removeSubstring(mozhegova::Text & text, size_t begin, size_t end)
  {
    for (auto & wordEntry : text)
    {
      using namespace std::placeholders;
      auto iter = std::remove_if(wordEntry.second.begin(), wordEntry.second.end(), std::bind(cmpBetween, _1, begin, end));
      wordEntry.second.erase(iter, wordEntry.second.end());
    }
    for (auto & wordEntry : text)
    {
      auto b = wordEntry.second.begin();
      auto e = wordEntry.second.end();
      using namespace std::placeholders;
      std::transform(b, e, b, std::bind(downLenNum, _1, begin, end));
    }
  }

  mozhegova::WordPos uppLenNum(const mozhegova::WordPos & pos, size_t n, size_t add)
  {
    if (pos.first >= n)
    {
      return {pos.first + add, pos.second};
    }
    return pos;
  }

  mozhegova::WordPos changeLenNum(const mozhegova::WordPos & pos, size_t n, size_t begin)
  {
    return {pos.first + n - begin, pos.second};
  }

  void insertTextTo(mozhegova::Text & text1, const mozhegova::Text & text2, size_t n, size_t begin, size_t end)
  {
    mozhegova::Text temp = extractSubstring(text2, begin, end);
    for (auto & wordEntry : text1)
    {
      auto b = wordEntry.second.begin();
      auto e = wordEntry.second.end();
      using namespace std::placeholders;
      std::transform(b, e, b, std::bind(uppLenNum, _1, n, end - begin));
    }
    for (auto & wordEntry : temp)
    {
      auto b = wordEntry.second.begin();
      auto e = wordEntry.second.end();
      using namespace std::placeholders;
      std::transform(b, e, std::back_inserter(text1[wordEntry.first]), std::bind(changeLenNum, _1, n, begin));
    }
  }

  bool isWordOnLine(const mozhegova::WordPos & pos, size_t line)
  {
    return pos.first == line;
  }

  void addWordToCombinedText(const std::pair< std::string, VectorWordPos > & wordEntry, size_t line, mozhegova::Text & combinedText)
  {
    auto b = wordEntry.second.begin();
    auto e = wordEntry.second.end();
    using namespace std::placeholders;
    std::copy_if(b, e, std::back_inserter(combinedText[wordEntry.first]), std::bind(isWordOnLine, _1, line));
  }

  mozhegova::WordPos reverseLenNum(const mozhegova::WordPos & pos, size_t maxLine)
  {
    return {maxLine - pos.first + 1, pos.second};
  }
}

void mozhegova::generateLinks(std::istream & in, Texts & texts)
{
  std::string textName, fileName;
  in >> textName >> fileName;
  std::ifstream file(fileName);
  if (!file.is_open())
  {
    throw std::runtime_error("<INVALID FILE>");
  }
  if (texts.find(textName) != texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text temp{};
  size_t line = 0;
  while (!file.eof())
  {
    ++line;
    std::string word;
    size_t num = 0;
    while (file.peek() != '\n' && file >> word)
    {
      ++num;
      temp[word].push_back({line, num});
    }
    file.ignore();
  }
  texts[textName] = std::move(temp);
}

void mozhegova::removeLinks(std::istream & in, Texts & texts)
{
  std::string textName;
  in >> textName;
  if (texts.find(textName) == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  texts.erase(textName);
}

void mozhegova::printLinks(std::istream & in, std::ostream & out, const Texts & texts)
{
  std::string textName;
  in >> textName;
  auto it = texts.find(textName);
  if (it == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  const Text & temp = it->second;
  auto maxWordIt = std::max_element(temp.cbegin(), temp.cend(), cmpMaxWordLen);
  size_t maxWordLen = maxWordIt->first.length() + 2;
  std::for_each(temp.cbegin(), temp.cend(), PrintWords{out, maxWordLen});
}

void mozhegova::printText(std::istream & in, std::ostream & out, const Texts & texts)
{
  std::string textName;
  in >> textName;
  auto it = texts.find(textName);
  if (it == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  const Text & temp = it->second;
  std::string reconstructed = reconstructText(temp);
  out << reconstructed << '\n';
}

void mozhegova::printTextInFile(std::istream & in, const Texts & texts)
{
  std::string fileName;
  in >> fileName;
  std::ofstream file(fileName);
  if (!file.is_open())
  {
    throw std::runtime_error("<INVALID FILE>");
  }
  printText(in, file, texts);
}

void mozhegova::mergeTexts(std::istream & in, Texts & texts)
{
  std::string newText, textName1, textName2;
  in >> newText >> textName1 >> textName2;
  auto it1 = texts.find(textName1);
  auto it2 = texts.find(textName2);
  auto it = texts.find(newText);
  if (it1 == texts.end() || it2 == texts.end() || it != texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  const Text & temp1 = it1->second;
  const Text & temp2 = it2->second;
  Text temp = temp1;
  size_t num = getMaxLineNum(temp) + 1;
  insertTextTo(temp, temp2, num, 1, getMaxLineNum(temp2) + 1);
  texts[newText] = std::move(temp);
}

void mozhegova::insertText(std::istream & in, Texts & texts)
{
  std::string textName1, textName2;
  size_t num = 0, begin = 0, end = 0;
  in >> textName1 >> num >> textName2 >> begin >> end;
  auto it1 = texts.find(textName1);
  auto it2 = texts.find(textName2);
  if (it1 == texts.end() || it2 == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text & temp1 = it1->second;
  const Text & temp2 = it2->second;
  if ((num > 1 + getMaxLineNum(temp1)) || (begin < 1) || (end > getMaxLineNum(temp2) + 1))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  insertTextTo(temp1, temp2, num, begin, end);
}

void mozhegova::removeLines(std::istream & in, Texts & texts)
{
  std::string textName;
  size_t begin = 0, end = 0;
  in >> textName >> begin >> end;
  auto it = texts.find(textName);
  if (it == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text & temp = it->second;
  if ((begin < 1) || (end > getMaxLineNum(temp) + 1))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  removeSubstring(temp, begin, end);
}

void mozhegova::moveText(std::istream & in, Texts & texts)
{
  std::string textName1, textName2;
  size_t num = 0, begin = 0, end = 0;
  in >> textName1 >> num >> textName2 >> begin >> end;
  auto it1 = texts.find(textName1);
  auto it2 = texts.find(textName2);
  if (it1 == texts.end() || it2 == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text & temp1 = it1->second;
  Text & temp2 = it2->second;
  if ((num > 1 + getMaxLineNum(temp1)) || (begin < 1) || (end > getMaxLineNum(temp2) + 1))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  insertTextTo(temp1, temp2, num, begin, end);
  removeSubstring(temp2, begin, end);
}

void mozhegova::sideMergeTexts(std::istream & in, Texts & texts)
{
  std::string newText, textName1, textName2;
  in >> newText >> textName1 >> textName2;
  auto it1 = texts.find(textName1);
  auto it2 = texts.find(textName2);
  auto it = texts.find(newText);
  if (it1 == texts.end() || it2 == texts.end() || it != texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  const Text & temp1 = it1->second;
  const Text & temp2 = it2->second;
  Text temp = temp1;
  size_t maxLines = std::max(getMaxLineNum(temp1), getMaxLineNum(temp2));
  for (size_t line = 1; line <= maxLines; ++line)
  {
    for (const auto & wordEntry : temp2)
    {
      addWordToCombinedText(wordEntry, line, temp);
    }
  }
  texts[newText] = std::move(temp);
}

void mozhegova::splitTexts(std::istream & in, Texts & texts)
{
  std::string textName, newText1, newText2;
  size_t num = 0;
  in >> textName >> num >> newText1 >> newText2;
  auto it = texts.find(textName);
  if (it == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text & temp = it->second;
  size_t end = getMaxLineNum(temp);
  if (num > end)
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text newTemp = extractSubstring(temp, num, end + 1);
  removeSubstring(temp, num, end + 1);
  texts[newText1] = std::move(temp);
  texts[newText2] = std::move(newTemp);
  texts.erase(textName);
}

void mozhegova::invertLines(std::istream & in, Texts & texts)
{
  std::string textName;
  in >> textName;
  auto it = texts.find(textName);
  if (it == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text & temp = it->second;
  size_t maxLine = getMaxLineNum(temp);
  for (auto & wordEntry : temp)
  {
    auto b = wordEntry.second.begin();
    auto e = wordEntry.second.end();
    using namespace std::placeholders;
    std::transform(b, e, b, std::bind(reverseLenNum, _1, maxLine));
  }
}

// void mozhegova::invertWords(std::istream & in, Texts & texts)
// {
//   std::string textName;
//   in >> textName;
//   auto it = texts.find(textName);
//   if (it == texts.end())
//   {
//     throw std::runtime_error("<INVALID COMMAND>");
//   }
//   Text & temp = it->second;
//   //??
// }

void mozhegova::replaceWord(std::istream & in, Texts & texts)
{
  std::string textName, oldWord, newWord;
  in >> textName >> oldWord >> newWord;
  auto it = texts.find(textName);
  if (it == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text & temp = it->second;
  auto wordIt = temp.find(oldWord);
  if (wordIt == temp.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  temp[newWord] = std::move(wordIt->second);
  temp.erase(wordIt);
}

void mozhegova::save(std::istream & in, const Texts & texts)
{
  std::string fileName;
  in >> fileName;
  std::ofstream file(fileName);
  if (!file.is_open())
  {
    throw std::runtime_error("<INVALID FILE>");
  }
  file << texts.size() << '\n';
  std::for_each(texts.cbegin(), texts.cend(), PrintText{file});
}

void mozhegova::loadFile(std::istream & in, Texts & texts)
{
  std::string fileName;
  in >> fileName;
  std::ifstream file(fileName);
  if (!file.is_open())
  {
    throw std::runtime_error("<INVALID FILE>");
  }
  size_t textsCount;
  file >> textsCount;
  for (size_t i = 0; i < textsCount; ++i)
  {
    std::string textName;
    size_t wordCount;
    file >> textName >> wordCount;
    Text & currText = texts[textName];
    std::string word;
    size_t line = 0;
    size_t j = 0;
    while (j < wordCount)
    {
      ++line;
      size_t num = 0;
      while (file.peek() != '\n' && file >> word)
      {
        ++num;
        currText[word].push_back({line, num});
        ++j;
      }
      file.ignore();
    }
  }
}

void mozhegova::printHelp(std::ostream & out)
{
  out << "Available commands:\n";
  out << "generatelinks <text name> <text file> - generate new links from file\n";
  out << "removelinks <text name> - delete text\n";
  out << "printlinks <text name> - print words from text with xref\n";
  out << "printtext <text name> - print text\n";
  out << "printtextinfile <text file> <text name> - print text in file\n";
  out << "merge <text result name> <text1 name> <text2 name> - merge two texts\n";
  out << "insert <text1 name> <num> <text2 name> <begin> <end> - insert part of the text into another\n";
  out << "removelines <text name> <begin> <end> - delete part of the text\n";
  out << "move <text1 name> <num> <text2 name> <begin> <end> - move part of the text into another\n";
  out << "sidemerge <text result name> <text1 name> <text2 name> - merge two texts along the lines\n";
  out << "split <text name> <num> <new text1 name> <new text2 name> - split the text into two texts\n";
  out << "invertlines <text name> - reverse the order of lines in the text\n";
  out << "invertwords <text name> - reverse the order of words in each line in the text\n";
  out << "replaceword <text name> <old word> <new word> - replace a word in the text with a new one \n";
  out << "save <file name> - save all texts\n";
  out << "--help - show this help\n";
}
