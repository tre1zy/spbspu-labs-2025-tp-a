#ifndef STREAM_GUARD_HPP
#define STREAM_GUARD_HPP

#include <ios>

namespace rychkov
{
  namespace input
  {
    class StreamGuard
    {
    public:
      inline StreamGuard(std::ios_base& in):
        stream_(in),
        flags_(in.flags()),
        width_(in.width()),
        precision_(in.precision())
      {}
      inline ~StreamGuard()
      {
        stream_.flags(flags_);
        stream_.width(width_);
        stream_.precision(precision_);
      }
    private:
      std::ios_base& stream_;
      std::ios::fmtflags flags_;
      size_t width_;
      size_t precision_;
    };
  }
}

#endif
