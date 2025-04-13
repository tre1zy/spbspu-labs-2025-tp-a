#ifndef FORMAT_GUARD_HPP
#define FORMAT_GUARD_HPP

#include <ios>

namespace filonova
{
  struct FormatGuard
  {
    FormatGuard(std::basic_ios< char > &s);
    ~FormatGuard();

  private:
    std::basic_ios< char > &s_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };

} // namespace filonova

#endif
