#ifndef WORD_FUNCTORS_HPP
#define WORD_FUNCTORS_HPP

#include <string>
#include <vector>
#include <map>
#include "index.hpp"

namespace trukhanov
{
  struct FindWord
  {
    const std::string& word;
    const ConcordanceIndex& index;
    const std::string& indexName;
    std::ostream& out;
    FindWord(const ConcordanceIndex& index_, const std::string& word_, const std::string& indexName_, std::ostream& out_);
    void operator()() const;
  };

  struct ReplaceWordFunctor
  {
    ConcordanceIndex& index;
    std::string oldWord;
    std::string newWord;
    void operator()();
  };

  struct ExportWordFunctor
  {
    ExportWordFunctor(const std::vector< std::string >& lines_, std::ostream& out_, const std::string& word_);
    void operator()(const std::map< std::string, std::set< std::size_t > >& index) const;
  private:
    const std::vector< std::string >& lines;
    std::ostream& out;
    const std::string& word;
  };

  struct LengthGreater
  {
    bool operator()(const std::string& a, const std::string& b) const;
  };

  struct LengthLess
  {
    bool operator()(const std::string& a, const std::string& b) const;
  };

  struct ExtractWord
  {
    std::string operator()(const std::pair< const std::string, std::set< std::size_t > >& pair) const;
  };

  struct OutputWord
  {
    std::ostream& out;
    std::string operator()(const std::string& word) const;
  };

  struct FindDifferentFrequencies
  {
    const ConcordanceIndex& index1;
    const ConcordanceIndex& index2;
    bool operator()(const std::string& word) const;
  };

  struct CompareByFrequencyDesc
  {
    bool operator()(const std::pair< std::string, std::size_t >& a, const std::pair< std::string, std::size_t >& b) const;
  };

  struct FrequencyCollector
  {
    std::pair< std::string, std::size_t > operator()(const IndexMap::value_type& entry) const;
  };
}

#endif
