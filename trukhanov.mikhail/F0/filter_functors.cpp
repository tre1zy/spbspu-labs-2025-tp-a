#include "filter_functors.hpp"
#include <algorithm>
#include <iterator>

bool trukhanov::InRange::operator()(std::size_t val) const
{
  return val >= from && val <= to;
}

std::size_t trukhanov::AdjustOffset::operator()(std::size_t val) const
{
  return val - offset;
}

void trukhanov::ProcessPair::operator()(const std::pair< const std::string, std::set< std::size_t > >& pair) const
{
  std::vector< std::size_t > filtered;
  std::copy_if(pair.second.begin(), pair.second.end(), std::back_inserter(filtered), trukhanov::InRange{ from, to });

  if (!filtered.empty())
  {
    std::set< std::size_t > adjusted;
    std::transform(filtered.begin(), filtered.end(), std::inserter(adjusted, adjusted.end()), trukhanov::AdjustOffset{ from - 1 });
    dest.index[pair.first] = std::move(adjusted);
  }
}

trukhanov::LineFilter::LineFilter(const ConcordanceIndex& s, ConcordanceIndex& d, std::size_t f,std::size_t t):
  from(f), to(t), src(s), dest(d)
{}

void trukhanov::LineFilter::operator()() const
{
  std::for_each(src.index.begin(), src.index.end(), ProcessPair{ from, to, src, dest });
}
