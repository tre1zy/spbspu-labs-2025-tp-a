#ifndef FUNCTORS
#define FUNCTORS
#include <unordered_map>
#include <string>

namespace shapkov
{
  struct isAnagram
  {
    std::string& word_;
    bool operator()(const std::string& word) const;
  };
  struct isPalindrome
  {
    size_t palindromesCnt = 0;
    bool operator()(const std::string& word);
  };
  void cleanWord(std::string& word);
  struct EntropyCalc
  {
    size_t size;
    double operator()(const std::pair< std::string, size_t >& word_pair) const;
  };
  struct ExtractFirst
  {
    std::string operator()(const std::pair< std::string, size_t >& p) const;
  };
  struct isPattern
  {
    std::string& pattern;
    size_t patternMatches = 0;
    bool operator()(const std::string& word);
  };
}

#endif
