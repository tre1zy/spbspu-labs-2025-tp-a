#include "xref.hpp"
#include <fstream>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

namespace amine
{

  bool positionLess::operator()(const Position& a, const Position& b) const
  {
    return (a.line < b.line) || (a.line == b.line && a.column < b.column);
  }

  std::ostream& operator<<(std::ostream& out, const Position& pos)
  {
    out << pos.line << ":" << pos.column;
    return out;
  }

  CrossRefSystem::CrossRefSystem()
  {
    indexes_ = std::map<std::string, Index>();
  }

   void readLines(std::ifstream& file, std::vector<std::string>& lines)
  {
    std::string line;
    if (!std::getline(file, line)) return;
    lines.push_back(line);
    readLines(file, lines);
  }

  void splitWords(std::istringstream& stream, std::vector<std::string>& words)
  {
    std::string word;
    if (!(stream >> word)) return;
    words.push_back(word);
    splitWords(stream, words);
  }

  void insertWordsRecursive(
    amine::Index& index,
    const std::vector<std::string>& words,
    size_t lineNum,
    size_t col
  )
  {
    if (col >= words.size()) return;
    index[words[col]].insert(amine::Position{ lineNum, col });
    insertWordsRecursive(index, words, lineNum, col + 1);
  }

  void processLinesRecursive(
    amine::Index& index,
    const std::vector<std::string>& lines,
    size_t lineNum
  )
  {
    if (lineNum >= lines.size()) return;

    std::istringstream stream(lines[lineNum]);
    std::vector<std::string> words;
    splitWords(stream, words);

    insertWordsRecursive(index, words, lineNum, 0);
    processLinesRecursive(index, lines, lineNum + 1);
  }
}

void CrossRefSystem::buildIndex(const std::string& indexName, const std::string& fileName)
{
  std::ifstream file(fileName);
  if (!file.is_open())
  {
    std::cout << "<FILE ERROR>\n";
    return;
  }

  if (indexes_.find(indexName) != indexes_.end())
  {
    std::cout << "<WRONG INDEX>\n";
    return;
  }

  std::vector<std::string> lines;
  std::string line;

  std::function<void()> readLines;
  readLines = [&]()
  {
    if (!std::getline(file, line)) return;
    lines.push_back(line);
    readLines();
  };
  readLines();

  Index index;

  std::function<void(size_t)> processLine;
  processLine = [&](size_t lineIdx)
  {
    if (lineIdx >= lines.size()) return;

    std::istringstream stream(lines[lineIdx]);
    std::vector<std::string> words;
    std::string word;

    std::function<void()> splitWords;
    splitWords = [&]()
    {
      if (!(stream >> word)) return;
      std::string cleaned;
      std::function<void(size_t)> clean;
      clean = [&](size_t i)
      {
        if (i >= word.size()) return;
        if (!std::ispunct(static_cast<unsigned char>(word[i])))
        {
          cleaned += word[i];
        }
        clean(i + 1);
      };
      clean(0);
      if (!cleaned.empty()) words.push_back(cleaned);
      splitWords();
    };
    splitWords();

    std::function<void(size_t)> insertWords;
    insertWords = [&](size_t col)
    {
      if (col >= words.size()) return;
      index[words[col]].insert(Position{lineIdx, col});
      insertWords(col + 1);
    };
    insertWords(0);
    processLine(lineIdx + 1);
  };
  processLine(0);

  indexes_.insert(std::make_pair(indexName, index));
}

void CrossRefSystem::deleteIndex(const std::string& indexName)
{
  auto it = indexes_.find(indexName);
  if (it == indexes_.end())
  {
    std::cout << "<WRONG INDEX>\n";
    return;
  }
  indexes_.erase(it);
}

void CrossRefSystem::containsWord(const std::string& indexName, const std::string& word) const
{
  auto it = indexes_.find(indexName);
  if (it == indexes_.end())
  {
    std::cout << "<WRONG INDEX>\n";
    return;
  }

  const Index& index = it->second;
  auto found = index.find(word);
  if (found != index.end())
  {
    std::cout << "<YES>\n";
  }
  else
  {
    std::cout << "<NO>\n";
  }
}

  void printPositionsRecursive(std::set<amine::Position>::const_iterator it,
                               std::set<amine::Position>::const_iterator end)
  {
    if (it == end) return;
    std::cout << " " << it->line << ":" << it->column;
    printPositionsRecursive(std::next(it), end);
  }

  void printIndexRecursive(std::map<std::string, std::set<amine::Position>>::const_iterator it,
                           std::map<std::string, std::set<amine::Position>>::const_iterator end)
  {
    if (it == end) return;

    std::cout << it->first << ":";
    printPositionsRecursive(it->second.begin(), it->second.end());
    std::cout << "\n";

    printIndexRecursive(std::next(it), end);
  }

void CrossRefSystem::printIndex(const std::string& indexName) const
{
  auto found = indexes_.find(indexName);
  if (found == indexes_.end() || found->second.empty())
  {
    std::cout << "<WRONG INDEX>\n";
    return;
  }

  std::function<void(std::map<std::string, std::set<Position>>::const_iterator,
                     std::map<std::string, std::set<Position>>::const_iterator)> printRecursive;

  printRecursive = [&](std::map<std::string, std::set<Position>>::const_iterator it,
                       std::map<std::string, std::set<Position>>::const_iterator end)
  {
    if (it == end) return;

    std::cout << it->first;

    std::function<void(std::set<Position>::const_iterator,
                       std::set<Position>::const_iterator)> printPositions;

    printPositions = [&](std::set<Position>::const_iterator pit,
                         std::set<Position>::const_iterator pend)
    {
      if (pit == pend)
      {
        std::cout << "\n";
        return;
      }
      std::cout << " " << pit->line << ":" << pit->column;
      printPositions(std::next(pit), pend);
    };

    printPositions(it->second.begin(), it->second.end());
    printRecursive(std::next(it), end);
  };

  printRecursive(found->second.begin(), found->second.end());
}

void CrossRefSystem::getPositions(const std::string& indexName, const std::string& word) const
{
  auto it = indexes_.find(indexName);
  if (it == indexes_.end())
  {
    std::cout << "<WRONG INDEX>\n";
    return;
  }

  const Index& index = it->second;
  auto wordIt = index.find(word);
  if (wordIt == index.end())
  {
    std::cout << "<NOT FOUND>\n";
    return;
  }

  const std::set<Position>& positions = wordIt->second;

  std::function<void(std::set<Position>::const_iterator, std::set<Position>::const_iterator)>
    printPositions;
  printPositions = [&](std::set<Position>::const_iterator it, std::set<Position>::const_iterator end)
  {
    if (it == end)
    {
      std::cout << "\n";
      return;
    }
    std::cout << it->line << ":" << it->column << " ";
    printPositions(std::next(it), end);
  };

  printPositions(positions.begin(), positions.end());
}
void CrossRefSystem::replaceWord(const std::string& indexName, const std::string& oldWord, const std::string& newWord)
{
  auto it = indexes_.find(indexName);
  if (it == indexes_.end())
  {
    std::cout << "<WRONG INDEX>\n";
    return;
  }

  Index& index = it->second;
  auto oldIt = index.find(oldWord);
  if (oldIt == index.end())
  {
    std::cout << "<NOT FOUND>\n";
    return;
  }

  std::set<Position> positions = oldIt->second;
  index.erase(oldIt);

  std::function<void(std::set<Position>::const_iterator, std::set<Position>::const_iterator)>
    insertNewWord;
  insertNewWord = [&](std::set<Position>::const_iterator it, std::set<Position>::const_iterator end)
  {
    if (it == end) return;
    index[newWord].insert(*it);
    insertNewWord(std::next(it), end);
  };

  insertNewWord(positions.begin(), positions.end());
}
void CrossRefSystem::mergeTexts(const std::string& newIndex, const std::string& index1, const std::string& index2)
{
  auto it1 = indexes_.find(index1);
  auto it2 = indexes_.find(index2);

  if (it1 == indexes_.end() || it2 == indexes_.end())
  {
    std::cout << "<WRONG INDEX>\n";
    return;
  }

  Index merged;
  const Index& i1 = it1->second;
  const Index& i2 = it2->second;

  std::function<void(std::map<std::string, std::set<Position>>::const_iterator)> insert1;
  insert1 = [&](std::map<std::string, std::set<Position>>::const_iterator it)
  {
    if (it == i1.end()) return;

    std::function<void(std::set<Position>::const_iterator)> insertSet;
    insertSet = [&](std::set<Position>::const_iterator pit)
    {
      if (pit == it->second.end()) return;
      merged[it->first].insert(*pit);
      insertSet(std::next(pit));
    };

    insertSet(it->second.begin());
    insert1(std::next(it));
  };

  insert1(i1.begin());

  size_t maxLine = 0;

  std::function<size_t(std::map<std::string, std::set<Position>>::const_iterator)> findMax;
  findMax = [&](std::map<std::string, std::set<Position>>::const_iterator it)
  {
    if (it == i1.end()) return maxLine;

    std::function<size_t(std::set<Position>::const_iterator)> maxInSet;
    maxInSet = [&](std::set<Position>::const_iterator pit)
    {
      if (pit == it->second.end()) return maxLine;
      if (pit->line > maxLine) maxLine = pit->line;
      return maxInSet(std::next(pit));
    };

    maxInSet(it->second.begin());
    return findMax(std::next(it));
  };

  maxLine = findMax(i1.begin()) + 1;

  std::function<void(std::map<std::string, std::set<Position>>::const_iterator)> insert2;
  insert2 = [&](std::map<std::string, std::set<Position>>::const_iterator it)
  {
    if (it == i2.end()) return;

    std::function<void(std::set<Position>::const_iterator)> insertSet;
    insertSet = [&](std::set<Position>::const_iterator pit)
    {
      if (pit == it->second.end()) return;
      Position shifted{ pit->line + maxLine, pit->column };
      merged[it->first].insert(shifted);
      insertSet(std::next(pit));
    };

    insertSet(it->second.begin());
    insert2(std::next(it));
  };

  insert2(i2.begin());

  indexes_.insert({ newIndex, merged });
}
void CrossRefSystem::insertText(const std::string& newIndex, const std::string& baseIndex, const std::string& insertIndex, size_t afterLine, size_t afterColumn)
{
  auto baseIt = indexes_.find(baseIndex);
  auto insertIt = indexes_.find(insertIndex);

  if (baseIt == indexes_.end() || insertIt == indexes_.end())
  {
    std::cout << "<WRONG INDEX>\n";
    return;
  }

  Index result;
  const Index& base = baseIt->second;
  const Index& insert = insertIt->second;

  bool validPosition = false;

  std::function<void(std::map<std::string, std::set<Position>>::const_iterator)> checkPos;
  checkPos = [&](std::map<std::string, std::set<Position>>::const_iterator it)
  {
    if (it == base.end()) return;
    std::function<void(std::set<Position>::const_iterator)> checkSet;
    checkSet = [&](std::set<Position>::const_iterator pit)
    {
      if (pit == it->second.end()) return;
      if (pit->line == afterLine && pit->column == afterColumn)
      {
        validPosition = true;
        return;
      }
      checkSet(std::next(pit));
    };
    checkSet(it->second.begin());
    checkPos(std::next(it));
  };

  checkPos(base.begin());
  if (!validPosition)
  {
    std::cout << "<INVALID POSITION>\n";
    return;
  }

  std::function<size_t(std::map<std::string, std::set<Position>>::const_iterator)> maxLineBase;
  maxLineBase = [&](std::map<std::string, std::set<Position>>::const_iterator it)
  {
    if (it == base.end()) return 0u;

    size_t maxLocal = 0;
    std::function<void(std::set<Position>::const_iterator)> findMax;
    findMax = [&](std::set<Position>::const_iterator pit)
    {
      if (pit == it->second.end()) return;
      if (pit->line > maxLocal) maxLocal = pit->line;
      findMax(std::next(pit));
    };

    findMax(it->second.begin());
    size_t maxRest = maxLineBase(std::next(it));
    return std::max(maxLocal, maxRest);
  };

  size_t insertShift = afterLine + 1;

  std::function<void(std::map<std::string, std::set<Position>>::const_iterator)> copyBaseBefore;
  copyBaseBefore = [&](std::map<std::string, std::set<Position>>::const_iterator it)
  {
    if (it == base.end()) return;

    std::function<void(std::set<Position>::const_iterator)> copyPos;
    copyPos = [&](std::set<Position>::const_iterator pit)
    {
      if (pit == it->second.end()) return;
      if (pit->line < insertShift ||
         (pit->line == insertShift && pit->column <= afterColumn))
      {
        result[it->first].insert(*pit);
      }
      copyPos(std::next(pit));
    };

    copyPos(it->second.begin());
    copyBaseBefore(std::next(it));
  };

  copyBaseBefore(base.begin());

  std::function<void(std::map<std::string, std::set<Position>>::const_iterator)> copyInsert;
  copyInsert = [&](std::map<std::string, std::set<Position>>::const_iterator it)
  {
    if (it == insert.end()) return;

    std::function<void(std::set<Position>::const_iterator)> copyPos;
    copyPos = [&](std::set<Position>::const_iterator pit)
    {
      if (pit == it->second.end()) return;
      Position shifted{ pit->line + insertShift, pit->column };
      result[it->first].insert(shifted);
      copyPos(std::next(pit));
    };

    copyPos(it->second.begin());
    copyInsert(std::next(it));
  };

  copyInsert(insert.begin());

  std::function<void(std::map<std::string, std::set<Position>>::const_iterator)> copyBaseAfter;
  copyBaseAfter = [&](std::map<std::string, std::set<Position>>::const_iterator it)
  {
    if (it == base.end()) return;

    std::function<void(std::set<Position>::const_iterator)> copyPos;
    copyPos = [&](std::set<Position>::const_iterator pit)
    {
      if (pit == it->second.end()) return;
      if (pit->line > afterLine || (pit->line == afterLine && pit->column > afterColumn))
      {
        Position shifted{ pit->line + insertShift, pit->column };
        result[it->first].insert(shifted);
      }
      else
      {
        result[it->first].insert(*pit);
      }
      copyPos(std::next(pit));
    };

    copyPos(it->second.begin());
    copyBaseAfter(std::next(it));
  };

  copyBaseAfter(base.begin());

  indexes_.insert({ newIndex, result });
}
void CrossRefSystem::extractText(const std::string& newIndex, const std::string& baseIndex,
                                 size_t startLine, size_t startCol, size_t endLine, size_t endCol)
{
  auto it = indexes_.find(baseIndex);
  if (it == indexes_.end())
  {
    std::cout << "<WRONG INDEX>\n";
    return;
  }

  if (startLine > endLine || (startLine == endLine && startCol > endCol))
  {
    std::cout << "<INVALID RANGE>\n";
    return;
  }

  const Index& base = it->second;
  Index result;

  std::function<void(std::map<std::string, std::set<Position>>::const_iterator)> copyInRange;
  copyInRange = [&](std::map<std::string, std::set<Position>>::const_iterator wordIt)
  {
    if (wordIt == base.end()) return;

    const std::string& word = wordIt->first;
    const std::set<Position>& positions = wordIt->second;

    std::function<void(std::set<Position>::const_iterator)> copyPos;
    copyPos = [&](std::set<Position>::const_iterator pit)
    {
      if (pit == positions.end()) return;

      const Position& pos = *pit;
      bool inRange =
        (pos.line > startLine || (pos.line == startLine && pos.column >= startCol)) &&
        (pos.line < endLine || (pos.line == endLine && pos.column <= endCol));

      if (inRange)
      {
        result[word].insert(pos);
      }

      copyPos(std::next(pit));
    };

    copyPos(positions.begin());
    copyInRange(std::next(wordIt));
  };

  copyInRange(base.begin());

  indexes_.insert({ newIndex, result });
}
void CrossRefSystem::repeatText(const std::string& newIndex, const std::string& baseIndex, size_t count)
{
  if (count == 0)
  {
    std::cout << "<INVALID N>\n";
    return;
  }

  auto it = indexes_.find(baseIndex);
  if (it == indexes_.end())
  {
    std::cout << "<WRONG INDEX>\n";
    return;
  }

  const Index& base = it->second;
  Index result;

  std::function<void(std::map<std::string, std::set<Position>>::const_iterator)> copyOriginal;
  copyOriginal = [&](std::map<std::string, std::set<Position>>::const_iterator it)
  {
    if (it == base.end()) return;

    std::function<void(std::set<Position>::const_iterator)> insertSet;
    insertSet = [&](std::set<Position>::const_iterator pit)
    {
      if (pit == it->second.end()) return;
      result[it->first].insert(*pit);
      insertSet(std::next(pit));
    };

    insertSet(it->second.begin());
    copyOriginal(std::next(it));
  };

  copyOriginal(base.begin());

  std::function<size_t(std::map<std::string, std::set<Position>>::const_iterator, size_t)> maxLine;
  maxLine = [&](std::map<std::string, std::set<Position>>::const_iterator it, size_t currentMax)
  {
    if (it == base.end()) return currentMax;

    std::function<size_t(std::set<Position>::const_iterator, size_t)> maxInSet;
    maxInSet = [&](std::set<Position>::const_iterator pit, size_t m)
    {
      if (pit == it->second.end()) return m;
      size_t newM = (pit->line > m ? pit->line : m);
      return maxInSet(std::next(pit), newM);
    };

    size_t maxHere = maxInSet(it->second.begin(), currentMax);
    return maxLine(std::next(it), maxHere);
  };

  size_t baseMaxLine = maxLine(base.begin(), 0);
  size_t offset = baseMaxLine + 1;
  size_t remaining = count - 1;

  std::function<void(size_t)> repeatCopy;
  repeatCopy = [&](size_t remainingTimes)
  {
    if (remainingTimes == 0) return;

    std::function<void(std::map<std::string, std::set<Position>>::const_iterator)> insertBlock;
    insertBlock = [&](std::map<std::string, std::set<Position>>::const_iterator it)
    {
      if (it == base.end()) return;

      std::function<void(std::set<Position>::const_iterator)> insertSet;
      insertSet = [&](std::set<Position>::const_iterator pit)
      {
        if (pit == it->second.end()) return;
        Position shifted{ pit->line + offset * (count - remainingTimes), pit->column };
        result[it->first].insert(shifted);
        insertSet(std::next(pit));
      };

      insertSet(it->second.begin());
      insertBlock(std::next(it));
    };

    insertBlock(base.begin());
    repeatCopy(remainingTimes - 1);
  };

  repeatCopy(remaining);
  indexes_.insert({ newIndex, result });
}
void CrossRefSystem::interleaveLines(const std::string& newIndex, const std::string& index1, const std::string& index2)
{
  auto it1 = indexes_.find(index1);
  auto it2 = indexes_.find(index2);

  if (it1 == indexes_.end() || it2 == indexes_.end())
  {
    std::cout << "<WRONG INDEX>\n";
    return;
  }

  const Index& i1 = it1->second;
  const Index& i2 = it2->second;
  Index result;

  std::map<size_t, std::vector<std::pair<std::string, size_t>>> lines1;
  std::map<size_t, std::vector<std::pair<std::string, size_t>>> lines2;

  std::function<void(std::map<std::string, std::set<Position>>::const_iterator, std::map<size_t, std::vector<std::pair<std::string, size_t>>>&)> flatten;
  flatten = [&](std::map<std::string, std::set<Position>>::const_iterator it, std::map<size_t, std::vector<std::pair<std::string, size_t>>>& lines)
  {
    if (it == i1.end() && &lines == &lines1) return flatten(i2.begin(), lines2);
    if (it == i2.end()) return;

    const std::string& word = it->first;
    const std::set<Position>& positions = it->second;

    std::function<void(std::set<Position>::const_iterator)> insertToLines;
    insertToLines = [&](std::set<Position>::const_iterator pit)
    {
      if (pit == positions.end()) return;
      lines[pit->line].push_back(std::make_pair(word, pit->column));
      insertToLines(std::next(pit));
    };

    insertToLines(positions.begin());

    flatten(std::next(it), lines);
  };

  flatten(i1.begin(), lines1);

  size_t maxLine1 = (lines1.empty() ? 0 : lines1.rbegin()->first);
  size_t maxLine2 = (lines2.empty() ? 0 : lines2.rbegin()->first);
  size_t maxLines = (maxLine1 > maxLine2 ? maxLine1 : maxLine2);
  size_t totalLines = maxLine1 + maxLine2 + 1;

  std::function<void(size_t, size_t)> mergeLines;
  mergeLines = [&](size_t l, size_t resLine)
  {
    if (l > maxLines) return;

    if (lines1.find(l) != lines1.end())
    {
      std::function<void(std::vector<std::pair<std::string, size_t>>::const_iterator)> insertWords;
      insertWords = [&](std::vector<std::pair<std::string, size_t>>::const_iterator it)
      {
        if (it == lines1[l].end()) return;
        result[it->first].insert(Position{ resLine, it->second });
        insertWords(std::next(it));
      };
      insertWords(lines1[l].begin());
      ++resLine;
    }

    if (lines2.find(l) != lines2.end())
    {
      std::function<void(std::vector<std::pair<std::string, size_t>>::const_iterator)> insertWords;
      insertWords = [&](std::vector<std::pair<std::string, size_t>>::const_iterator it)
      {
        if (it == lines2[l].end()) return;
        result[it->first].insert(Position{ resLine, it->second });
        insertWords(std::next(it));
      };
      insertWords(lines2[l].begin());
      ++resLine;
    }

    mergeLines(l + 1, resLine);
  };

  mergeLines(0, 0);

  indexes_.insert({ newIndex, result });
}
void CrossRefSystem::swapWords(const std::string& indexName, const std::string& word1, const std::string& word2)
{
  auto it = indexes_.find(indexName);
  if (it == indexes_.end())
  {
    std::cout << "<WRONG INDEX>\n";
    return;
  }

  Index& index = it->second;

  auto it1 = index.find(word1);
  auto it2 = index.find(word2);

  if (it1 == index.end() || it2 == index.end())
  {
    std::cout << "<NOT FOUND>\n";
    return;
  }

  std::set<Position> positions1 = it1->second;
  std::set<Position> positions2 = it2->second;

  index.erase(word1);
  index.erase(word2);

  std::function<void(std::set<Position>::const_iterator)> insert1;
  insert1 = [&](std::set<Position>::const_iterator it)
  {
    if (it == positions1.end()) return;
    index[word2].insert(*it);
    insert1(std::next(it));
  };

  std::function<void(std::set<Position>::const_iterator)> insert2;
  insert2 = [&](std::set<Position>::const_iterator it)
  {
    if (it == positions2.end()) return;
    index[word1].insert(*it);
    insert2(std::next(it));
  };

  insert1(positions1.begin());
  insert2(positions2.begin());
}

void CrossRefSystem::reverseText(const std::string& newIndex, const std::string& baseIndex)
{
  auto it = indexes_.find(baseIndex);
  if (it == indexes_.end())
  {
    std::cout << "<WRONG INDEX>\n";
    return;
  }

  const Index& base = it->second;
  if (base.empty())
  {
    std::cout << "<EMPTY>\n";
    return;
  }

  size_t maxLine = 0;

  std::function<size_t(std::map<std::string, std::set<Position>>::const_iterator)> findMax;
  findMax = [&](std::map<std::string, std::set<Position>>::const_iterator it)
  {
    if (it == base.end()) return maxLine;

    std::function<void(std::set<Position>::const_iterator)> maxInSet;
    maxInSet = [&](std::set<Position>::const_iterator pit)
    {
      if (pit == it->second.end()) return;
      if (pit->line > maxLine) maxLine = pit->line;
      maxInSet(std::next(pit));
    };

    maxInSet(it->second.begin());
    return findMax(std::next(it));
  };

  maxLine = findMax(base.begin());

  Index reversed;

  std::function<void(std::map<std::string, std::set<Position>>::const_iterator)> reverseInsert;
  reverseInsert = [&](std::map<std::string, std::set<Position>>::const_iterator it)
  {
    if (it == base.end()) return;

    std::function<void(std::set<Position>::const_iterator)> insertSet;
    insertSet = [&](std::set<Position>::const_iterator pit)
    {
      if (pit == it->second.end()) return;
      Position flipped{ maxLine - pit->line, pit->column };
      reversed[it->first].insert(flipped);
      insertSet(std::next(pit));
    };

    insertSet(it->second.begin());
    reverseInsert(std::next(it));
  };

  reverseInsert(base.begin());

  indexes_.insert({ newIndex, reversed });
}

void CrossRefSystem::saveIndex(const std::string& indexName, const std::string& fileName)
{
  auto it = indexes_.find(indexName);
  if (it == indexes_.end())
  {
    std::cout << "<WRONG INDEX>\n";
    return;
  }

  std::ofstream out(fileName);
  if (!out.is_open())
  {
    std::cout << "<FILE ERROR>\n";
    return;
  }

  const Index& index = it->second;

  std::function<void(std::map<std::string, std::set<Position>>::const_iterator)> saveWords;
  saveWords = [&](std::map<std::string, std::set<Position>>::const_iterator it)
  {
    if (it == index.end()) return;

    out << it->first;

    std::function<void(std::set<Position>::const_iterator)> savePositions;
    savePositions = [&](std::set<Position>::const_iterator pit)
    {
      if (pit == it->second.end()) return;
      out << " " << pit->line << ":" << pit->column;
      savePositions(std::next(pit));
    };

    savePositions(it->second.begin());
    out << '\n';

    saveWords(std::next(it));
  };

  saveWords(index.begin());
}

void CrossRefSystem::loadIndex(const std::string& indexName, const std::string& fileName)
{
  std::ifstream file(fileName);
  if (!file.is_open())
  {
    std::cout << "<FILE ERROR>\n";
    return;
  }

  Index index;

  std::function<void()> readLines;
  readLines = [&]()
  {
    std::string line;
    if (!std::getline(file, line)) return;

    std::istringstream stream(line);
    std::string word;
    stream >> word;

    std::function<void()> readPositions;
    readPositions = [&]()
    {
      std::string pos;
      if (!(stream >> pos)) return;

      size_t delim = pos.find(':');
      size_t lineNum = std::stoul(pos.substr(0, delim));
      size_t col = std::stoul(pos.substr(delim + 1));

      index[word].insert(Position{ lineNum, col });

      readPositions();
    };

    readPositions();
    readLines();
  };

  readLines();

  indexes_[indexName] = index;
}

void CrossRefSystem::reconstructText(const std::string& indexName, const std::string& fileName)
{
  auto it = indexes_.find(indexName);
  if (it == indexes_.end())
  {
    std::cout << "<WRONG INDEX>\n";
    return;
  }

  const Index& index = it->second;
  if (index.empty())
  {
    std::cout << "<EMPTY>\n";
    return;
  }

  std::vector<std::vector<std::string>> grid;

  std::function<void(std::map<std::string, std::set<Position>>::const_iterator)> placeWords;
  placeWords = [&](std::map<std::string, std::set<Position>>::const_iterator it)
  {
    if (it == index.end()) return;

    const std::string& word = it->first;
    const std::set<Position>& positions = it->second;

    std::function<void(std::set<Position>::const_iterator)> placeEach;
    placeEach = [&](std::set<Position>::const_iterator pit)
    {
      if (pit == positions.end()) return;

      size_t line = pit->line;
      size_t col = pit->column;

      if (grid.size() <= line)
      {
        grid.resize(line + 1);
      }

      if (grid[line].size() <= col)
      {
        grid[line].resize(col + 1, "");
      }

      grid[line][col] = word;

      placeEach(std::next(pit));
    };

    placeEach(positions.begin());
    placeWords(std::next(it));
  };

  placeWords(index.begin());

  std::ofstream out(fileName);
  if (!out.is_open())
  {
    std::cout << "<FILE ERROR>\n";
    return;
  }

  std::function<void(size_t)> writeLines;
  writeLines = [&](size_t i)
  {
    if (i >= grid.size()) return;

    std::function<void(size_t)> writeColumns;
    writeColumns = [&](size_t j)
    {
      if (j >= grid[i].size()) return;
      if (!grid[i][j].empty())
        out << grid[i][j];
      else
        out << " ";
      if (j + 1 < grid[i].size()) out << " ";
      writeColumns(j + 1);
    };

    writeColumns(0);
    out << '\n';
    writeLines(i + 1);
  };

  writeLines(0);
}
