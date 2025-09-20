#include "streamguard.hpp"

smirnov::streamguard::streamguard(std::basic_ios< char >& s):
  s_(s),
  precision_(s.precision()),
  flags_(s.flags())
  {}

smirnov::streamguard::~streamguard()
{
    s_.precision(precision_);
    s_.flags(flags_);
}
