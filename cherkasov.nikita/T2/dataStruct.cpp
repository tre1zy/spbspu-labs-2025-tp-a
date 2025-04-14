#include "dataStruct.hpp"
#include "streamGuard.hpp"
#include <sstream>
#include <cmath>

namespace cherkasov
{
  bool DataStruct::operator<(const DataStruct & other) const
  {
    const auto mag = [](const std::complex<double>& c)
    {
      return std::abs(c);
    };
    if (mag(key1) != mag(other.key1))
      return mag(key1) < mag(other.key1);
    if (key2 != other.key2)
      return key2 < other.key2;
    return key3.size() < other.key3.size();
  }

  std::istream & operator>>(std::istream & in, DelimiterIO && d)
  {
    char c;
    if (!(in >> c) || c != d.exp)
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream & operator>>(std::istream & in, LabelIO && l)
  {
    std::string tmp;
    if (!(in >> tmp) || tmp != l.exp)
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream & operator>>(std::istream & in, ComplexIO && io)
  {
    StreamGuard g(in);
    in >> DelimiterIO{ '#' } >> DelimiterIO{ 'c' } >> DelimiterIO{ '(' };
    double re = 0.0;
    double im = 0.0;
    in >> re;
    char sign = in.peek();
    if (sign == '+' || sign == '-')
    {
      in >> im;
    }
    in >> DelimiterIO{ ')' };
    if (in)
    {
      io.c = std::complex< double >(re, im);
    }
    return in;
  }

  std::istream & operator>>(std::istream & in, RationalIO && io)
  {
    in >> DelimiterIO{ '(' } >> LabelIO{ "N" } >> io.rat.first
       >> DelimiterIO{ ':' } >> LabelIO{ "D" } >> io.rat.second
       >> DelimiterIO{ ':' } >> DelimiterIO{ ')' };
    return in;
  }

  std::istream & operator>>(std::istream & in, StringIO && io)
  {
    StreamGuard guard(in);
    in >> std::noskipws >> DelimiterIO{ '"' };
    char ch;
    while (in >> ch && ch != '"')
    {
      if (ch == '\n')
      {
        in.setstate(std::ios::failbit);
        break;
      }
      io.s += ch;
    }
    return in;
  }

  std::istream & operator>>(std::istream & in, DataStruct & obj)
  {
    std::istream::sentry s(in);
    if (!s) return in;
    DataStruct temp;
    bool k1 = false, k2 = false, k3 = false;
    in >> DelimiterIO{ '(' };
    while (!(k1 && k2 && k3) && in)
    {
      std::string label;
      in >> label;
      if (label == ":key1" && !k1)
      {
        in >> ComplexIO{ temp.key1 };
        k1 = true;
      }
      else if (label == ":key2" && !k2)
      {
        in >> RationalIO{ temp.key2 };
        k2 = true;
      }
      else if (label == ":key3" && !k3)
      {
        in >> StringIO{ temp.key3 };
        k3 = true;
      }
      else
      {
        in.setstate(std::ios::failbit);
      }
    }
    in >> DelimiterIO{ ':' } >> DelimiterIO{ ')' };
    if (in)
    {
      obj = temp;
    }
    return in;
  }

  std::ostream & operator<<(std::ostream & out, const DataStruct & obj)
  {
    out << "(:key1 #c(" << obj.key1.real();
    if (obj.key1.imag() >= 0)
      out << "+" << obj.key1.imag();
    else
      out << obj.key1.imag();
    out << "):";
    out << "key2 (:N" << obj.key2.first << ":D " << obj.key2.second << ":):";
    out << "key3 \"" << obj.key3 << "\":)";
    return out;
  }
}
