#include "Alphabet.hpp"
#include <iterator>
#include <numeric>
#include <vector>

void holodilov::Alphabet::load(const Dictionary& dictionary)
{
  std::vector< std::string > vecTranslationStrings(dictionary.dict.size());
  std::transform(dictionary.dict.begin(), dictionary.dict.end(), vecTranslationStrings.begin(), dictPairToString);

  std::string strAllTranslations = std::accumulate(vecTranslationStrings.begin(), vecTranslationStrings.end(), std::string());

  alphabet_ = std::accumulate(strAllTranslations.begin(), strAllTranslations.end(), std::set< char >(), accumulateAlphabet);
}

std::string holodilov::Alphabet::dictPairToString(const std::pair< std::string, std::list< std::string > >& pair)
{
  return joinList(pair.second);
}

std::set< char > holodilov::Alphabet::accumulateAlphabet(std::set< char >&& setAlphabet, char ch)
{
  setAlphabet.insert(ch);
  return setAlphabet;
}

std::string holodilov::Alphabet::joinList(const std::list< std::string >& list)
{
  return std::accumulate(list.begin(), list.end(), std::string(""));
}

std::ostream& holodilov::operator<<(std::ostream& out, const Alphabet& alphabet)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  using ostreamIter = std::ostream_iterator< char >;
  std::copy(alphabet.alphabet_.begin(), alphabet.alphabet_.end(), ostreamIter(out));
  return out;
}

std::istream& holodilov::operator>>(std::istream& in, Alphabet& alphabet)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  using istreamIter = std::istream_iterator< char >;
  std::copy(istreamIter(in), istreamIter(), std::inserter(alphabet.alphabet_, alphabet.alphabet_.end()));
  return in;
}


bool holodilov::Alphabet::operator==(const Alphabet& rhs) const
{
  return alphabet_ == rhs.alphabet_;
}
