#include "index.hpp"
#include "split_functors.hpp"
#include <iostream>
#include <set>
#include <iterator>
#include <algorithm>

trukhanov::WordEntry::WordEntry(const std::pair< const std::string, std::set< size_t > >& p):
  word(p.first), lines(p.second)
{}

std::ostream& trukhanov::operator<<(std::ostream& out, const std::set< size_t >& s)
{
  if (s.empty())
  {
    return out;
  }

  auto it = s.begin();
  out << *it;
  ++it;

  if (it != s.end())
  {
    out << " ";
    std::copy(it, std::prev(s.end()), std::ostream_iterator< size_t >(out, " "));
    out << *std::prev(s.end());
  }

  return out;
}

std::ostream& trukhanov::operator<<(std::ostream& out, const trukhanov::WordEntry& entry)
{
  out << entry.word << " : " << entry.lines;
  return out;
}

std::istream& trukhanov::operator>>(std::istream& in, std::vector< std::string >& args)
{
  std::string line;
  if (!std::getline(in, line))
  {
    return in;
  }

  RecursiveSplitter splitter{ args };
  splitter(line.begin(), line.end());

  return in;
}
