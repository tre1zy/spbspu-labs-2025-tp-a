#include "output-struct.hpp"
#include "data-struct.hpp"

namespace aleksandrov
{
  namespace detail
  {
    std::ostream& operator<<(std::ostream& out, const DoubleO& dest)
    {
      std::ostream::sentry sentry(out);
      if (!sentry)
      {
        return out;
      }
      return out << std::fixed << std::setprecision(1) << dest.ref << 'd';
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
      return out << ":)";
    }

    std::ostream& operator<<(std::ostream& out, const StringO& dest)
    {
      std::ostream::sentry sentry(out);
      if (!sentry)
      {
        return out;
      }
      return out << '"' << dest.ref << '"';
    }
  }
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    detail::iofmtguard fmtguard(out);
    out << "(:";
    out << "key1 " << detail::DoubleO{ dest.key1 } << ':';
    out << "key2 " << detail::RationalO{ dest.key2 } << ':';
    out << "key3 " << detail::StringO{ dest.key3 };
    return out << ":)";
  }
}

