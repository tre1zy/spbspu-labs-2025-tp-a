#include "output_functors.hpp"
#include <algorithm>
#include <iterator>
#include <iostream>

void trukhanov::ShowEntry::operator()(const std::pair< std::string, std::size_t >& pair) const
{
  out << pair.first << ' ' << pair.second << '\n';
}

std::string trukhanov::MergeLinesFunctor::operator()()
{
  std::string line1;
  if (i < lines1.size())
  {
    line1 = lines1[i];
  }
  else
  {
    line1 = "";
  }

  std::string line2;
  if (i < lines2.size())
  {
    line2 = lines2[i];
  }
  else
  {
    line2 = "";
  }
  ++i;
  return line1 + " " + line2;
}

std::string trukhanov::FormatIndexEntry::operator()(const std::pair< const std::string, ConcordanceIndex >& pair) const
{
  const std::string& file = pair.second.sourceFile;
  std::string result;

  if (file.empty())
  {
    result = pair.first + ": <NO FILE>";
  }
  else
  {
    result = pair.first + ": " + file;
  }
  return result;
}

std::size_t trukhanov::AddOffset::operator()(std::size_t val) const
{
  return val + offset;
}

void trukhanov::MergeWithOffset::operator()(const std::pair< const std::string, std::set< std::size_t > >& pair) const
{
  std::set< std::size_t > adjusted;
  std::transform(
    pair.second.begin(),
    pair.second.end(),
    std::inserter(adjusted, adjusted.end()),
    AddOffset{ offset }
  );

  auto& target = dest.index[pair.first];
  target.insert(adjusted.begin(), adjusted.end());
}


trukhanov::WordEntry trukhanov::PairToWordEntry::operator()(const IndexMap::value_type& p) const
{
  return WordEntry(p);
}

std::string trukhanov::ShowEntryToString::operator()(const std::pair< std::string, std::size_t >& p) const
{
  return p.first + " : " + std::to_string(p.second);
}
