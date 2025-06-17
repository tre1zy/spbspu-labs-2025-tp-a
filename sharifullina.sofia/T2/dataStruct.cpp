#include "dataStruct.hpp"
#include "scopeGuard.hpp"

#include <complex>
#include <string>
#include <iomanip>
#include <sstream>

namespace
{
  struct DelimiterIO
  {
    char exp;
  };

  struct ComplexIO
  {
    std::complex<double>& ref;
  };

  struct RationalIO
  {
    std::pair<long long, unsigned long long>& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    char c = 0;
    in >> c;
    if (in && c != dest.exp)
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, ComplexIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    double real = 0.0;
    double imag = 0.0;
    char hash = 0, c = 0, lparen = 0, rparen = 0, colon = 0;
    in >> hash >> c >> lparen >> real >> imag >> rparen >> colon;

    if (in && hash == '#' && c == 'c' && lparen == '(' && rparen == ')' && colon == ':')
    {
      dest.ref = std::complex<double>(real, imag);
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, RationalIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    char lparen = 0, colon1 = 0, nchar = 0, colon2 = 0, dchar = 0, colon3 = 0, rparen = 0, colon4 = 0;
    long long num = 0;
    unsigned long long den = 0;

    in >> lparen >> colon1 >> nchar >> num >> colon2 >> dchar >> den >> colon3 >> rparen >> colon4;

    if (in &&
        lparen == '(' && colon1 == ':' && nchar == 'N' &&
        colon2 == ':' && dchar == 'D' && colon3 == ':' &&
        rparen == ')' && colon4 == ':')
    {
      dest.ref = std::make_pair(num, den);
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, StringIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    char quote = 0;
    in >> quote;
    if (quote != '"')
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    std::getline(in, dest.ref, '"');
    return in;
  }
}

bool sharifullina::DataStruct::operator<(const DataStruct& other) const
{
  if (std::abs(key1) != std::abs(other.key1))
  {
    return std::abs(key1) < std::abs(other.key1);
  }

  const double lhsRatio = static_cast<double>(key2.first) / key2.second;
  const double rhsRatio = static_cast<double>(other.key2.first) / other.key2.second;
  if (lhsRatio != rhsRatio)
  {
    return lhsRatio < rhsRatio;
  }

  return key3.length() < other.key3.length();
}

std::istream& sharifullina::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) return in;

  DataStruct result;
  std::string key;
  bool hasKey1 = false, hasKey2 = false, hasKey3 = false;

  in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };

  for (int i = 0; i < 3 && in; ++i)
  {
    in >> key;
    if (key == "key1")
    {
      hasKey1 = static_cast<bool>(in >> ComplexIO{ result.key1 });
    }
    else if (key == "key2")
    {
      hasKey2 = static_cast<bool>(in >> RationalIO{ result.key2 });
    }
    else if (key == "key3")
    {
      hasKey3 = static_cast<bool>(in >> StringIO{ result.key3 });
    }
    else
    {
      in.setstate(std::ios::failbit);
    }

    in >> DelimiterIO{ ':' };
  }

  in >> DelimiterIO{ ')' };

  if (in && hasKey1 && hasKey2 && hasKey3)
  {
    dest = result;
  }
  else
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::ostream& sharifullina::operator<<(std::ostream& out, const DataStruct& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry) return out;

  sharifullina::iofmtguard guard(out);
  out << std::fixed << std::setprecision(1) << std::showpoint;
  out << "(:key1 #c(" << dest.key1.real() << " " << dest.key1.imag() << ")"
      << ":key2 (:N " << dest.key2.first << ":D " << dest.key2.second << ":)"
      << ":key3 \"" << dest.key3 << "\":)";
  return out;
}
