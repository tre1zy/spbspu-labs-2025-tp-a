#ifndef FUNCTORS
#define FUNCTORS
#include <unordered_map>
#include <string>

namespace shapkov
{
  struct isAnagram
  {
    std::string word_;
    isAnagram(const std::string word);
    bool operator()(const std::string word);
  };
  bool isPalindrome(const std::string word);
  void cleanWord(std::string& word);
  struct EntropyCalc
  {
    size_t size;
    double operator()(std::pair< std::string, size_t >);
  };
}

#endif
