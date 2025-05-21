#include "functors.hpp"
#include <algorithm>
#include <regex>
#include <cmath>

bool shapkov::isAnagram::operator()(const std::string& word) const
{
  return word.size() == wrd.size() && std::is_permutation(word.begin(), word.end(), wrd.begin());
}

bool shapkov::isPalindrome::operator()(const std::string& word)
{
  bool isPal = std::equal(word.begin(), word.begin() + word.size() / 2, word.rbegin());
  if (isPal)
  {
    palindromesCnt++;
  }
  return isPal;
}

void shapkov::cleanWord(std::string& word)
{
  std::regex nonAlpha("[^a-zA-Z`]");
  word = std::regex_replace(word, nonAlpha, "");
  std::transform(word.begin(), word.end(), word.begin(), ::tolower);
}

double shapkov::EntropyCalc::operator()(const std::pair< std::string, size_t >& word_pair) const
{
  double wordProbability = static_cast< double >(word_pair.second) / size;
  return wordProbability * log2(wordProbability);
}

std::string shapkov::ExtractFirst::operator()(const std::pair< std::string, size_t >& p) const
{
  return p.first;
}

bool shapkov::isPattern::operator()(const std::string& word)
{
  std::regex patt(pattern);
  bool isPatt = std::regex_match(word, patt);
  if (isPatt)
  {
    patternMatches++;
  }
  return isPatt;
}

bool shapkov::CompareByFreq(const std::pair< std::string, size_t >& p1, const std::pair< std::string, size_t >& p2)
{
  return p1.second < p2.second;
}
