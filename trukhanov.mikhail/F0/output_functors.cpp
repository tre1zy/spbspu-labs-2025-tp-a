#include "output_functors.hpp"
#include <algorithm>
#include <iterator>
#include <sstream>

void trukhanov::ShowIndexEntry::operator()(const std::pair< const std::string, std::set< size_t > >& entry) const
{
  out << entry.first << ' ';
  std::copy(entry.second.begin(), entry.second.end(), std::ostream_iterator< std::size_t >(out, " "));
  out << '\n';
}

void trukhanov::ShowEntry::operator()(const std::pair< std::string, std::size_t >& pair) const
{
  out << pair.first << ' ' << pair.second << '\n';
}


void trukhanov::FileWriter::operator()(const std::pair< const std::string, std::set< std::size_t > >& entry) const
{
  out << entry.first << ' ';
  std::copy(entry.second.begin(), entry.second.end(), std::ostream_iterator< std::size_t >(out, " "));
  out << '\n';
}

std::string trukhanov::MergeLinesFunctor::operator()()
{
  std::string l1 = (i < lines1.size()) ? lines1[i] : "";
  std::string l2 = (i < lines2.size()) ? lines2[i] : "";
  ++i;
  return l1 + " " + l2;
}

std::string trukhanov::FormatIndexEntry::operator()(const std::pair< const std::string, ConcordanceIndex >& pair) const
{
  const std::string& file = pair.second.sourceFile;
  return pair.first + ": " + (file.empty() ? "<NO FILE>" : file);
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
    std::inserter(adjusted,adjusted.end()),
    trukhanov::AddOffset{ offset });
  dest.index[pair.first].insert(adjusted.begin(), adjusted.end());
}
