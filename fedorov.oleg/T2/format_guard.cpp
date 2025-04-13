#include "format_guard.hpp"

namespace fedorov {
FormatGuard::FormatGuard(std::ios &stream)
    : stream_(stream),
      flags_(stream.flags()),
      fill_(stream.fill()),
      precision_(stream.precision())
{
}

FormatGuard::~FormatGuard()
{
  stream_.flags(flags_);
  stream_.fill(fill_);
  stream_.precision(precision_);
}
} // namespace fedorov