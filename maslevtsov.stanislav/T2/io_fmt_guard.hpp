#ifndef IO_FMT_GUARD
#define IO_FMT_GUARD

#include <iomanip>

namespace maslevtsov {
  class IOFmtGuard
  {
  public:
    IOFmtGuard(std::basic_ios< char >& s);
    ~IOFmtGuard();

  private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };
}

#endif
