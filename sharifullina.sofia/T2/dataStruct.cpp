#include "dataStruct.hpp"
#include <vector>
#include <complex>
#include <string>
#include <iostream>
#include <iomanip>
#include <utility>
#include <limits>
#include <cmath>
#include "scopeGuard.hpp"

namespace
{
  struct DelimiterIO
  {
    char exp;
  };

  struct ComplexIO
  {
    std::complex< double > & ref;
  };

  struct RationalIO
  {
    std::pair< long long, unsigned long long > & ref;
  };

  struct StringIO
  {
    std::string & ref;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO && dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char c = 0;
    in >> c;
    if (in && c != dest.exp)
      in.setstate(std::ios::failbit);

    return in;
  }

  std::istream& operator>>(std::istream& in, ComplexIO && dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    double real = 0.0, imag = 0.0;
    in >> DelimiterIO{ '#' } >> DelimiterIO{ 'c' };
    in >> DelimiterIO{ '(' };
    in >> real >> imag;
    in >> DelimiterIO{ ')' };
    if (in)
    {
      dest.ref = std::complex<double>(real, imag);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, RationalIO && dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' } >> DelimiterIO{ 'N' } >> dest.ref.first;
    in >> DelimiterIO{ ':' } >> DelimiterIO{ 'D' } >> dest.ref.second >> DelimiterIO{ ':' };
    in >> DelimiterIO{ ')' } >> DelimiterIO{ ':' };
    return in;
  }

  std::istream& operator>>(std::istream& in, StringIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
  }
}

std::istream & sharifullina::operator>>(std::istream & in, sharifullina::DataStruct & dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct input;
  {
    std::vector< std::string > keys(3);
    in >> DelimiterIO{ '(' };
    for (size_t i = 0; i < 3; i++)
    {
      in >> DelimiterIO{ ':' };
      in >> keys[i];
      if (keys[i] == "key1")
      {
        in >> ComplexIO{ input.key1 };
      }
      else if (keys[i] == "key2")
      {
        in >> RationalIO{ input.key2 };
      }
      else if (keys[i] == "key3")
      {
        in >> StringIO{ input.key3 };
      }
      else
      {
        in.setstate(std::ios::failbit);
        return in;
      }
    }
    in >> DelimiterIO{ ':' };
    in >> DelimiterIO{ ')' };
  }
  if (in)
  {
    dest = input;
  }
  return in;
}

std::ostream& sharifullina::operator<<(std::ostream& out, const sharifullina::DataStruct & dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  sharifullina::IofmtGuard guard(out);
  out << std::fixed << std::setprecision(1);

  out << "(:key1 #c(" << dest.key1.real() << " " << dest.key1.imag() << ")";
  out << ":key2 (:N " << dest.key2.first << ":D " << dest.key2.second << ":)";
  out << ":key3 \"" << dest.key3 << "\":)";

  return out;
}

bool sharifullina::DataStruct::operator<(const DataStruct& other) const
{
  const double abs1 = std::abs(key1);
  const double abs2 = std::abs(other.key1);

  if (abs1 != abs2)
  {
    return abs1 < abs2;
  }

  const double lhsRatio = static_cast< double >(key2.first) / key2.second;
  const double rhsRatio = static_cast< double >(other.key2.first) / other.key2.second;

  if (lhsRatio != rhsRatio)
  {
    return lhsRatio < rhsRatio;
  }
  return key3.length() < other.key3.length();
}
