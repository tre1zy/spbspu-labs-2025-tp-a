#ifndef XREF_HPP
#define XREF_HPP

#include <string>
#include <set>
#include <map>

namespace amine
{
  struct Position
  {
    size_t line, column;
  };

  struct positionLess
  {
    bool operator()(const Position& a, const Position& b) const;
  };

  using Index = std::map<std::string, std::set<Position, positionLess>>;

  class CrossRefSystem
  {
  public:
    CrossRefSystem();

    void buildIndex(const std::string& indexName, const std::string& fileName);
    void deleteIndex(const std::string& indexName);
    void containsWord(const std::string& indexName, const std::string& word);
    void printIndex(const std::string& indexName) const;
    void getPositions(const std::string& indexName, const std::string& word) const;
    void mergeTexts(const std::string& newIndex, const std::string& index1, const std::string& index2);
    void insertText(const std::string& newIndex, const std::string& baseIndex, const std::string& insertIndex
      , size_t afterLine, size_t afterColumn);
    void extractText(const std::string& newIndex, const std::string& baseIndex
      , size_t startLine, size_t startCol, size_t endLine, size_t endCol);
    void replaceWord(const std::string& indexName, const std::string& oldWord, const std::string& newWord);
    void repeatText(const std::string& newIndex, const std::string& baseIndex, size_t N);
    void interleaveLines(const std::string& newIndex, const std::string& index1, const std::string& index2);
    void swapWords(const std::string& indexName, const std::string& word1, const std::string& word2);
    void reverseText(const std::string& newIndex, const std::string& baseIndex);
    void saveIndex(const std::string& indexName, const std::string& filename) const;
    void loadIndex(const std::string& indexName, const std::string& fileName);
    void reconstructText(const std::string& indexName, const std::string& filename) const;

  private:
    std::map<std::string, Index> indexes_;
  };

  std::ostream& operator<<(std::ostream& out, const Position& pos);
}
