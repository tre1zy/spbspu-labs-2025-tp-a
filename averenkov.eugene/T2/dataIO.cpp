#include "dataIO.hpp"
#include <sstream>

namespace DataIO
{
  std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char c = 0;
    if (in >> c && c != dest.exp)
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, CharIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return in >> DelimiterIO{'\''} >> dest.ref >> DelimiterIO{'\''};
  }

  std::istream& operator>>(std::istream& in, RationalIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    long long n = 0;
    unsigned long long d = 1;
    in >> DelimiterIO{'('} >> DelimiterIO{':'} >> LabelIO{"N"} >> n
       >> DelimiterIO{':'} >> LabelIO{"D"} >> d >> DelimiterIO{':'} >> DelimiterIO{')'};
    if (in)
    {
      dest.ref = {n, d};
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, StringIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
  }

  std::istream& operator>>(std::istream& in, LabelIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    std::string label;
    if (in >> label && label != dest.exp)
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }
}
