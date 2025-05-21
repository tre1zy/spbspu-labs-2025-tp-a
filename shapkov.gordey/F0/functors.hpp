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
}

#endif
