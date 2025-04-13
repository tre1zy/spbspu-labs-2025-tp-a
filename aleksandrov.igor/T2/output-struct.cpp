#include "output-struct.hpp"
#include "data-struct.hpp"

namespace aleksandrov
{
  std::ostream& operator<<(std::ostream& out, const DoubleO& dest)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    out << std::fixed << std::setprecision(1) << dest.ref << 'd';
    return out;
  }

  std::ostream& operator<<(std::ostream& out, const RationalO& dest)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    out << "(:N " << dest.ref.first;
    out << ":D " << dest.ref.second;
    out << ":)";
    return out;
  }

  std::ostream& operator<<(std::ostream& out, const StringO& dest)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    out << '"' << dest.ref << '"';
    return out;
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& dest)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    iofmtguard fmtguard(out);
    out << "(:" << "key1 " << DoubleO{ dest.key1 };
    out << ":key2 " << RationalO{ dest.key2 };
    out << ":key3 " << StringO{ dest.key3 } << ":)";
    return out;
  }
}

