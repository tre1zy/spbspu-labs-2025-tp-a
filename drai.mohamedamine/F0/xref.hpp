#ifndef XREF_HPP
#define XREF_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <map>
#include <set>
#include <vector>
#include <utility>
#include <memory>

namespace amine
{
  struct Position
  {
    size_t line;
    size_t column;

    Position(size_t l, size_t c):
      line(l),
      column(c)
    {}

    bool operator<(const Position& other) const
    {
      return (line < other.line) || (line == other.line && column < other.column);
    }

    friend std::ostream& operator<<(std::ostream& os, const Position& pos)
    {
      os << pos.line << ":" << pos.column;
      return os;
    }
  };

  using PositionSet = std::set<Position>;
  using WordMap = std::unordered_map<std::string, PositionSet>;

  struct Index
  {
    WordMap words;
    size_t totalLines;

    Index():
      words(),
      totalLines(0)
    {}
  };

  class CrossRefSystem
  {
  public:
    void buildIndex(const std::string& indexName, const std::string& fileName);
    void deleteIndex(const std::string& indexName);
    void containsWord(const std::string& indexName, const std::string& word) const;
    void printIndex(const std::string& indexName) const;
    void getPositions(const std::string& indexName, const std::string& word) const;
    void mergeTexts(const std::string& newIndex, const std::string& index1, const std::string& index2);
    void insertText(const std::string& newIndex, const std::string& baseIndex,
      const std::string& insertIndex, size_t afterLine, size_t afterColumn);
    void extractText(const std::string& newIndex, const std::string& baseIndex,
      size_t startLine, size_t startCol, size_t endLine, size_t endCol);
    void replaceWord(const std::string& indexName, const std::string& oldWord, const std::string& newWord);
    void repeatText(const std::string& newIndex, const std::string& baseIndex, size_t n);
    void interleaveLines(const std::string& newIndex, const std::string& index1, const std::string& index2);
    void swapWords(const std::string& indexName, const std::string& word1, const std::string& word2);
    void reverseText(const std::string& newIndex, const std::string& baseIndex);
    void saveIndex(const std::string& indexName, const std::string& filename) const;
    void loadIndex(const std::string& indexName, const std::string& filename);
    void reconstructText(const std::string& indexName, const std::string& filename) const;

  private:
    std::map<std::string, Index> indices_;
  };

}
