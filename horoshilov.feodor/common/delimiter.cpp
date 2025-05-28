#include "delimiter.h"
#include "guard.h"

namespace horoshilov
{
  std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    Guard guard(in);

    char c = 0;
    if (in >> c && std::tolower(c) != dest.exp)
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }
}