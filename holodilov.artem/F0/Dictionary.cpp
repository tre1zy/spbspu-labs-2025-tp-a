#include "Dictionary.hpp"

#include <iterator>
#include <map>

std::ostream& holodilov::operator<<(std::ostream& out, const Dictionary& dict)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  out << dict.name << " " << dict.lang << "\n";

  using OstreamIterator = std::ostream_iterator< std::string >;

  auto beginIter = dict.dict.begin();
  while (beginIter != dict.dict.end())
  {
    out << beginIter->first << " ";
    std::copy(std::begin(beginIter->second), std::end(beginIter->second), OstreamIterator(out, " "));
    out << "\n";
  }
  return out;
}

std::istream& holodilov::operator>>(std::istream& in, Dictionary& dict)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  std::string name;
  std::string lang;
  in >> name;
  if (in.peek() == '\n')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  in >> lang;
  if (in.peek() != '\n')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  Dictionary dictTemp(std::move(name), std::move(lang));

  std::string englishWord;
  while (!in.eof())
  {
    in >> englishWord;
    dictTemp.dict[englishWord] = std::list< std::string >();
    std::string translation;
    while (in.peek() != '\n')
    {
      in >> translation;
      dictTemp.dict[englishWord].push_back(translation);
    }
  }
  if (!in)
  {
    dict = dictTemp;
  }
  return in;
}