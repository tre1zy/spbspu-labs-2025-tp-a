#ifndef SCOPEGUARD_HPP
#define SCOPEGUARD_HPP
#include <ios>

namespace dribas
{
  class StreamGuard
  {
  public:
    explicit StreamGuard(std::basic_ios< char >&);
    ~StreamGuard();
  private:
    std::basic_ios< char >::fmtflags fmtflags_;
    std::streamsize width_;
    std::streamsize precision_;
    char fill_;
    std::basic_ios< char >& stream_;
  };
}

#endif
