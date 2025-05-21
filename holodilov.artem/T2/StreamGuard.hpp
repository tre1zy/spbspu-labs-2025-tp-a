#ifndef STREAM_GUARD_HPP
#define STREAM_GUARD_HPP

#include <ios>

namespace holodilov
{
  struct StreamGuard
  {
  public:
    explicit StreamGuard(std::basic_ios< char >& ios);
    ~StreamGuard();

  private:
    std::basic_ios< char >& ios_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmtFlags_;
    char fill_;
  };
}

#endif //STREAM_GUARD_HPP
