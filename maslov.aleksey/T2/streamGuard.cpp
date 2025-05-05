#include "streamGuard.hpp"

maslov::StreamGuard::StreamGuard(std::basic_ios< char > & s):
  width_(s.width()),
  precision_(s.precision()),
  fmt_(s.flags()),
  s_(s),
  fill_(s.fill())
{}

maslov::StreamGuard::~StreamGuard()
{
  s_.width(width_);
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}
