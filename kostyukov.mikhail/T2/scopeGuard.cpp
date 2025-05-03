#include "scopeGuard.hpp"
#include <ios>

kostyukov::ScopeGuard::ScopeGuard(std::basic_ios< char > &s) :
  s_(s),
  fmt_(s.flags()),
  precision_(s.precision()),
  width_(s.width()),
  fill_(s.fill())
{}

kostyukov::ScopeGuard::~ScopeGuard()
{
  s_.flags(fmt_);
  s_.precision(precision_);
  s_.width(width_);
  s_.fill(fill_);
}
