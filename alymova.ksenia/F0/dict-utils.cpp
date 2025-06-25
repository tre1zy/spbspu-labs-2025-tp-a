#include <algorithm>
#include <iterator>
#include "dict-utils.hpp"

namespace
{
  using namespace alymova;

  std::pair< std::string, std::list< std::string > > returnDictPair(DictPairWrapper pair)
  {
    return pair.p;
  }
  std::pair< std::string, alymova::Dictionary > returnDictSetPair(DictSetPairWrapper pair)
  {
    return pair.p;
  }
  alymova::DictPairWrapper returnDictPairWrapper(std::pair< std::string, std::list< std::string > > pair)
  {
    return DictPairWrapper{pair};
  }
  alymova::DictSetPairWrapper returnDictSetPairWrapper(std::pair< std::string, Dictionary > pair)
  {
    return DictSetPairWrapper{pair};
  }
}
std::istream& alymova::operator>>(std::istream& in, std::list< std::string >& list)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  size_t size;
  in >> size;
  if (size == 0)
  {
    return in;
  }
  std::list< std::string > tested;
  auto begin = std::istream_iterator< std::string >(in);
  auto end = std::istream_iterator< std::string >();
  std::copy_n(begin, size, std::back_inserter(tested));
  if (in.get() != '\n')
  {
    if (in.eof())
    {
      in.clear(in.rdstate() ^ std::ios::eofbit);
    }
    in.setstate(std::ios::failbit);
  }
  if (in && tested.size() == size)
  {
    list = tested;
  }
  return in;
}

std::istream& alymova::operator>>(std::istream& in, Dictionary& dict)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  size_t size;
  in >> size;
  if (!in)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::list< DictPairWrapper > tested_list;
  auto begin = std::istream_iterator< DictPairWrapper >(in);
  auto end = std::istream_iterator< DictPairWrapper >();
  std::copy_n(begin, size, std::back_inserter(tested_list));
  if (in && tested_list.size() == size)
  {
    std::transform(tested_list.begin(), tested_list.end(), std::inserter(dict, dict.end()), returnDictPair);
  }
  return in;
}
std::istream& alymova::operator>>(std::istream& in, DictSet& set)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  DictSet tested;
  std::list< DictSetPairWrapper > tested_list;
  auto begin = std::istream_iterator< DictSetPairWrapper >(in);
  auto end = std::istream_iterator< DictSetPairWrapper >();
  std::transform(begin, end, std::inserter(tested, tested.end()), returnDictSetPair);
  if (in.eof())
  {
    set = tested;
  }
  return in;
}
std::istream& alymova::operator>>(std::istream& in, DictPairWrapper& pair)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  DictPairWrapper tested;
  in >> tested.p.first >> tested.p.second;
  if (in)
  {
    pair = tested;
  }
  return in;
}
std::istream& alymova::operator>>(std::istream& in, DictSetPairWrapper& pair)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  DictSetPairWrapper tested;
  in >> tested.p.first >> tested.p.second;
  if (in)
  {
    pair = tested;
  }
  return in;
}
std::ostream& alymova::operator<<(std::ostream& out, const std::list< std::string >& list)
{
  std::ostream::sentry s(out);
  if (!s || list.empty())
  {
    return out;
  }
  std::list< std::string > copy(list);
  copy.sort();
  std::copy_n(copy.begin(), copy.size() - 1, std::ostream_iterator< std::string >(out, " "));
  out << *(--copy.end());
  return out;
}
std::ostream& alymova::operator<<(std::ostream& out, const Dictionary& dict)
{
  std::ostream::sentry s(out);
  if (!s || dict.empty())
  {
    return out;
  }
  std::transform(dict.begin(), --dict.end(), std::ostream_iterator< DictPairWrapper >(out, "\n"), returnDictPairWrapper);
  out << returnDictPairWrapper(*(--dict.end()));
  return out;
}
std::ostream& alymova::operator<<(std::ostream& out, const DictSet& set)
{
  std::ostream::sentry s(out);
  if (!s || set.empty())
  {
    return out;
  }
  std::transform(set.begin(), --set.end(), std::ostream_iterator< DictSetPairWrapper >(out, "\n"), returnDictSetPairWrapper);
  out << returnDictSetPairWrapper(*(--set.end()));
  return out;
}
std::ostream& alymova::operator<<(std::ostream& out, const DictPairWrapper& pair)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  out << pair.p.first << ' ' << pair.p.second.size();
  if (!pair.p.second.empty())
  {
    out << ' ' << pair.p.second;
  }
  return out;
}
std::ostream& alymova::operator<<(std::ostream& out, const DictSetPairWrapper& pair)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  out << pair.p.first << ' ' << pair.p.second.size();
  if (!pair.p.second.empty())
  {
    out << '\n' << pair.p.second;
  }
  return out;
}
