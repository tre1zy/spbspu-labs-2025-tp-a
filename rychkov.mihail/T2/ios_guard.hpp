#ifndef IOS_GUARD_HPP
#define IOS_GUARD_HPP

#include <ios>

namespace rychkov
{
  class IosGuard
  {
  public:
    inline IosGuard(std::ios_base& in):
      ios_(in),
      flags_(in.flags()),
      width_(in.width()),
      precision_(in.precision())
    {}
    inline ~IosGuard()
    {
      ios_.flags(flags_);
      ios_.width(width_);
      ios_.precision(precision_);
    }
  private:
    std::ios_base& ios_;
    std::ios::fmtflags flags_;
    size_t width_;
    size_t precision_;
  };
}

#endif
