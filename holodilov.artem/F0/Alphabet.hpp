#ifndef ALPHABET_HPP
#define ALPHABET_HPP

#include <algorithm>
#include <set>
#include "Dictionary.hpp"

namespace holodilov
{
  class Alphabet
  {
  public:
    void load(const Dictionary& dictionary);

    bool operator==(const Alphabet& rhs) const;

    friend std::ostream& operator<<(std::ostream& out, const Alphabet& alphabet);
    friend std::istream& operator>>(std::istream& in, Alphabet& alphabet);


  private:
    std::set< char > alphabet_;

    static std::string dictPairToString(const std::pair< std::string, std::list< std::string > >& pair);

    static std::set< char > accumulateAlphabet(std::set< char >&& setAlphabet, char ch);

    static std::string joinList(const std::list< std::string >& list);

  };

  std::ostream& operator<<(std::ostream& out, const Alphabet& alphabet);

  std::istream& operator>>(std::istream& in, Alphabet& alphabet);
}

#endif