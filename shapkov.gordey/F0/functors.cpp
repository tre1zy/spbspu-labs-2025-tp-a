#include "functors.hpp"
#include <algorithm>
#include <regex>
#include <cmath>

shapkov::isAnagram::isAnagram(const std::string word):
  word_(word)
{}

bool shapkov::isAnagram::operator()(const std::string word)
{
  return word.size() == word_.size() && std::is_permutation(word.begin(), word.end(), word_.begin());
}

bool shapkov::isPalindrome(const std::string word)
{
  return std::equal(word.begin(), word.begin() + word.size() / 2, word.rbegin());
}

void shapkov::cleanWord(std::string& word) {
  std::regex nonAlpha("[^a-zA-Z`]");
  word = std::regex_replace(word, nonAlpha, "");
  std::transform(word.begin(), word.end(), word.begin(), ::tolower);
}

double shapkov::EntropyCalc::operator()(std::pair< std::string, size_t > word_pair)
{
  double wordProbability = static_cast< double >(word_pair.second) / size;
  return wordProbability * log2(wordProbability);
}
