#include "split_functors.hpp"
#include <algorithm>
#include <iterator>

std::string trukhanov::LineSplitter::operator()(const std::string& word) const
{
  wordMap[word].insert(lineNumber);
  return word;
}

std::size_t trukhanov::SplitWords::operator()(std::size_t lineNumber) const
{
  const std::string& line = lines.at(lineNumber);

  std::vector< std::string > words;
  RecursiveSplitter splitter{ words };
  splitter(line.begin(), line.end());

  std::transform(words.begin(), words.end(), words.begin(), LineSplitter{ wordMap, lineNumber + 1 });

  return lineNumber;
}

bool trukhanov::IsSpace::operator()(unsigned char ch) const
{
  return std::isspace(ch);
}

bool trukhanov::NotSpace::operator()(unsigned char ch) const
{
  return !std::isspace(ch);
}

void trukhanov::RecursiveSplitter::operator()(std::string::const_iterator first, std::string::const_iterator last) const
{
  first = std::find_if(first, last, NotSpace{});
  if (first == last)
  {
    return;
  }

  auto wordEnd = std::find_if(first, last, IsSpace{});
  out.emplace_back(first, wordEnd);
  (*this)(wordEnd, last);
}

std::size_t trukhanov::SplitAndAdd::operator()(std::size_t lineNum) const
{
  const std::string& line = lines[lineNum - 1];

  std::vector< std::string > words;
  trukhanov::RecursiveSplitter splitter{ words };
  splitter(line.begin(), line.end());

  std::transform(words.begin(), words.end(), words.begin(), trukhanov::LineSplitter{ idx.index, lineNum });
  return lineNum;
}
