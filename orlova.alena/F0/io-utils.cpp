#include "io-utils.h"
#include <algorithm>
#include <map>
#include <list>

struct DictionaryPair
{
  std::pair< std::string, std::list< std::string > > p;
};

std::pair< std::string, std::list< std::string > > returnDictionaryPair(DictionaryPair pair)
{
  return pair.p;
}

std::istream& orlova::operator>>(std::istream& in, Dictionary& dict)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  /*size_t size;
  in >> size;
  if (!in)
  {
      in.setstate(std::ios::failbit);
      return in;
  }
  std::list< DictionaryPair > tested_list;
  auto begin = std::istream_iterator< DictionaryPair >(in);
  auto end = std::istream_iterator< DictionaryPair >();
  std::copy_n(begin, size, std::back_inserter(tested_list));
  if (in && tested_list.size() == size)
  {
      std::transform(tested_list.begin(), tested_list.end(), std::inserter(dict, dict.end()), returnDictionaryPair);
  }*/
  return in;
}
