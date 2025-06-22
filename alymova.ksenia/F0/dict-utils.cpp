#include "dict-utils.hpp"

std::istream& alymova::operator>>(std::istream& in, std::list< std::string >& list)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  size_t cnt;
  in >> cnt;
  std::list< std::string > tested;
  for (size_t i = 0; i < cnt && in; i++)
  {
    std::string word;
    in >> word;
    tested.push_back(word);
  }
  if (!in)
  {
    return in;
  }
  list = tested;
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
  auto it = copy.begin();
  out << *(it++);
  for (; it != copy.end(); it++)
  {
    out << ' ' << *it;
  }
  return out;
}
