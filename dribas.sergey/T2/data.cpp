#include "data.hpp"

#include <iomanip>
#include <sstream>

#include "streamGuard.hpp"

namespace dribas
{
  bool compare(const Data& lhs, const Data& rhs)
  {
    if (lhs.key1 != rhs.key1) {
      return lhs.key1 < rhs.key1;
    }
    if (lhs.key2 != rhs.key2) {
      return lhs.key2 < rhs.key2;
    }
    return lhs.key3.length() < rhs.key3.length();
  }

  std::ostream& operator<<(std::ostream& out, const DoubleO&& dbl)
  {
    std::ostream::sentry sentry(out);
    if (!sentry) {
      return out;
    }
    StreamGuard guard(out);

    out << "key1 ";
    out << std::setprecision(2) << dbl.ref;
    out << "d";
    return out;
  }

  std::ostream& operator<<(std::ostream& out, const UllO&& ull)
  {
    std::ostream::sentry sentry(out);
    if (!sentry) {
      return out;
    }
    StreamGuard guard(out);

    out << "key2 0x";
    out << std::hex << std::uppercase << ull.ref;
    return out;
  }

  std::ostream& operator<<(std::ostream& out, const StringO&& str)
  {
    std::ostream::sentry sentry(out);
    if (!sentry) {
      return out;
    }

    StreamGuard guard(out);
    out << "key3 ";
    out << '"';
    out << str.ref;
    out << '"';
    return out;
  }

  std::istream& operator>>(std::istream& in, DoubleI&& dbl)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    return in >> dbl.ref >> DelimiterIO{ 'd' };
  }

  std::istream& operator>>(std::istream& in, UllI&& ull)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    return in >> DelimiterIO{ '0' } >> DelimiterIO{ 'x' } >> ull.ref;
  }

  std::istream& operator>>(std::istream& in, StringI&& str)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    return std::getline(in >> DelimiterIO{ '"' }, str.ref, '"');
  }

  std::istream& operator>>(std::istream& in, DelimiterIO&& delimiter)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }

    char c = '0';
    in >> c;
    if (in && std::tolower(c) != delimiter.exp) {
      in.setstate(std::ios::failbit);
    }

    return in;
  }

  std::istream& operator>>(std::istream& in, LabelIO&& labl)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }

    std::string label;
    if ((in >> label) && label != labl.exp) {
      in.setstate(std::ios::failbit);
    }

    return in;
  }

  std::istream& operator>>(std::istream& in, Data& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    Data temp;
    in >> DelimiterIO{'('} >> DelimiterIO{':'};

    for (int i = 0; i < 3; ++i) {
      std::string key;
      in >> key;
      if (key != "key1" && key != "key2" && key != "key3") {
        in.setstate(std::ios::failbit);
        return in;
      }
      in >> DelimiterIO{':'};
      if (key == "key1") {
        in >> DoubleI{temp.key1};
      } else if (key == "key2") { 
        in >> UllI{temp.key2};
      } else if ((key == "key3")) {
        in >> StringI{temp.key3};
      }
      in >> DelimiterIO{':'};
    }
    in >> DelimiterIO{')'};
    if (in) {
      dest = temp;
    }
      return in;
  }

  std::ostream& operator<<(std::ostream& out, const Data& data)
  {
    std::ostream::sentry sentry(out);
    if (!sentry) {
      return out;
    }
    StreamGuard guard(out);

    out <<  "(:";
    out << DoubleO{ data.key1 };
    out << ':';
    out << UllO{ data.key2 };
    out << ':';
    out << StringO{ data.key3 };
    out << ":)";

    return out;
  }
}