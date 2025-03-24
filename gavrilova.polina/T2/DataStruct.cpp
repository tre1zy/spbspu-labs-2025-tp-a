#include "DataStruct.hpp"
#include <iomanip>
#include "IOStreamGuard.hpp"

namespace gavrilova {

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    char c;
    in >> c;
  if (in && (std::tolower(c) != dest.exp)) {
        in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, DoubleIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    return in >> dest.ref >> DelimiterIO{'d'};
  }

  std::istream& operator>>(std::istream& in, LongLongIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    return in >> dest.ref >> DelimiterIO{'l'} >> DelimiterIO{'l'};
  }

  std::istream& operator>>(std::istream& in, StringIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
  }

  std::istream& operator>>(std::istream& in, LabelIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    std::string data;
    if ((in >> StringIO{ data }) && (data != dest.exp)) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, DataStruct& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }

    DataStruct input;
    {
      using sep = DelimiterIO;
      using label = LabelIO;
      using ll = LongLongIO;
      using dbl = DoubleIO;
      using str = StringIO;

      in >> sep{ ':' };
      in >> label{ "key1" } >> sep{ ' ' } >> dbl{ input.key1 };
      in >> sep{ ':' };
      in >> label{ "key2" } >> sep{ ' ' } >> ll{ input.key2 };
      in >> sep{ ':' };
      in >> label{ "key3" } >> sep{ ' ' } >> str{ input.key3 };
    }

    if (in) {
      dest = input;
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
      std::ostream::sentry sentry(out);
      if (!sentry) {
          return out;
      }
      IOStreamGuard fmtguard(out);
      out << "(:key1 " << src.key1 << "ull:key2 " << src.key2 << ":" << "key3 \"" << src.key3 << "\":)";
      return out;
  }

}
