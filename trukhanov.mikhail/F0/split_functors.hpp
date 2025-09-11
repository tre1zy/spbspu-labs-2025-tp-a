#ifndef SPLIT_FUNCTORS_HPP
#define SPLIT_FUNCTORS_HPP

#include <string>
#include <map>
#include <set>
#include "index.hpp"

namespace trukhanov
{
  struct LineSplitter
  {
    IndexMap& wordMap;
    std::size_t lineNumber;
    std::string operator()(const std::string& word) const;
  };

  struct SplitWords
  {
    std::vector< std::string >& lines;
    IndexMap& wordMap;
    std::size_t operator()(std::size_t lineNumber) const;
  };

  bool isSpace(unsigned char ch);

  bool notSpace(unsigned char ch);

  struct RecursiveSplitter
  {
    std::vector< std::string >& out;
    void operator()(std::string::const_iterator first, std::string::const_iterator last) const;
  };

  struct SplitAndAdd
  {
    ConcordanceIndex& idx;
    std::vector< std::string >& lines;
    std::size_t operator()(std::size_t lineNum) const;
  };
}

#endif
