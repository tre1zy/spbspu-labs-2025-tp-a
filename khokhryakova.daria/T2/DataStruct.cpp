#include "DataStruct.h"
#include <iomanip>
#include <cmath>
#include <limits>
namespace khokhryakova
{
  std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char c = '\0' ;
    if ((in >> c) && (c != dest.exp))
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, ComplexIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    double real = 0.0;
    double imag = 0.0;
    in >> DelimiterIO{ '#' } >> DelimiterIO{ 'c' } >> DelimiterIO{ '(' };
    in >> real >> imag >> DelimiterIO{ ')' };
    if (in)
    {
      dest.ref = { real, imag };
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, RationalIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    long long N = 0;
    unsigned long long D = 1;
    in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' } >> DelimiterIO{ 'N' } >> N;
    in >> DelimiterIO{ ':' } >> DelimiterIO{ 'D' } >> D >> DelimiterIO{ ':' } >> DelimiterIO{ ')' };
    if (in)
    {
      dest.ref = { N, D };
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
    std::string temp;
    in >> DelimiterIO{ '"' };
    std::getline(in, temp, '"');
    if (in)
    {
      dest.ref = temp;
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, DataStruct& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    DataStruct temp;
    bool key1 = false, key2 = false, key3 = false;
    in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };
    for (size_t i = 0; i < 3; ++i)
    {
      std::string key;
      if ((in >> key) && (key.size() == 4) && key.substr(0, 3) == "key")
      {
        switch (key.back())
        {
        case '1':
          in >> ComplexIO{ temp.key1 };
          key1 = true;
          break;
        case '2':
          in >> RationalIO{ temp.key2 };
          key2 = true;
          break;
        case '3':
          in >> StringIO{ temp.key3 };
          key3 = true;
          break;
        default:
          in.setstate(std::ios::failbit);
        }
      }
      else
      {
        in.setstate(std::ios::failbit);
      }
      in >> DelimiterIO{ ':' };
    }
    in >> DelimiterIO{ ')' };
    if (in && key1 && key2 && key3)
    {
      dest = temp;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& src)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    std::ios_base::fmtflags flags = out.flags();
    out << std::fixed << std::setprecision(1);
    out << "(:key1 #c(" << src.key1.real() << " " << src.key1.imag() << ")";
    out << ":key2 (:N " << src.key2.first << ":D " << src.key2.second << ":)";
    out << ":key3 \"" << src.key3 << "\":)";
    out.flags(flags);
    return out;
  }

  bool operator<(const DataStruct& a, const DataStruct& b)
  {
    double a_mod = std::abs(a.key1);
    double b_mod = std::abs(b.key1);
    if (a_mod != b_mod)
    {
      return a_mod < b_mod;
    }
    double a_ratio = static_cast<double>(a.key2.first) / a.key2.second;
    double b_ratio = static_cast<double>(b.key2.first) / b.key2.second;
    if (a_ratio != b_ratio)
    {
      return a_ratio < b_ratio;
    }
    if (a.key3.length() != b.key3.length())
    {
      return a.key3.length() < b.key3.length();
    }
    return a.key3 < b.key3;
  }

}
