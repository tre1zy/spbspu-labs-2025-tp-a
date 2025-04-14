#include "stream_guard.hpp"

shak::StreamGuard::StreamGuard(std::basic_ios< char > &s):
  s_(s),
  fill_(s.fill()),
  precision_(s.precision()),
  flags_(s.flags())
{}

shak::StreamGuard::~StreamGuard()
{
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(flags_);
}
