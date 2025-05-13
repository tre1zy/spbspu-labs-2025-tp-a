#include "stream-guard.hpp"

belyaev::StreamGuard::StreamGuard(std::basic_ios< char >& s):
  fill_(s.fill()),
  width_(s.width()),
  precision_(s.precision()),
  s_(s),
  flags_(s.flags())
{}

belyaev::StreamGuard::~StreamGuard()
{
  s_.precision(precision_);
  s_.flags(flags_);
}
