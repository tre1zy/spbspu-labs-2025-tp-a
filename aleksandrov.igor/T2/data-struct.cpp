#include "data-struct.hpp"
#include <iostream>
#include <string>
#include <iomanip>

namespace aleksandrov
{
  std::istream& operator>>(std::istream& in, DelimeterIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char c = '\0';
    in >> c;
    if (in && c != dest.exp)
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, DoubleIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return in >> dest.ref >> DelimeterIO{ 'd' };
  }

  std::istream& operator>>(std::istream& in, RationalIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    using sep = DelimeterIO;
    using ll = LongLongIO;
    using ull = UnsignedLongLongIO;
    in >> sep{ '(' } >> sep{ ':' };
    in >> sep{ 'N' } >> ll{ dest.ref.first };
    in >> sep { ':' };
    in >> sep{ 'D' } >> ull{ dest.ref.second };
    in >> sep{ ':' } >> sep{ ')' };
    return in;
  }

  std::istream& operator>>(std::istream& in, LongLongIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return in >> dest.ref;
  }

  std::istream& operator>>(std::istream& in, UnsignedLongLongIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return in >> dest.ref;
  }

  std::istream& operator>>(std::istream& in, StringIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return std::getline(in >> DelimeterIO{ '"' }, dest.ref, '"');
  }

  std::istream& operator>>(std::istream& in, LabelIO& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    std::string data;
    if ((in >> data) && (data != dest.exp))
    {
      in.setstate(std::ios::failbit);
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
    DataStruct input;
    {
      using sep = DelimeterIO;
      using dbl = DoubleIO;
      using rat = RationalIO;
      using str = StringIO;
      in >> sep{ '(' } >> sep{ ':' };
      for (size_t i = 0; i < 3; ++i)
      {
        std::string key;
        in >> key;
        if (key == "key1")
        {
          in >> dbl{ input.key1 };
        }
        else if (key == "key2")
        {
          in >> rat{ input.key2 };
        }
        else if (key == "key3")
        {
          in >> str{ input.key3 };
        }
        in >> sep{ ':' };
      }
      in >> sep{ ')' };
    }
    if (in)
    {
      dest = input;
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const DoubleIO& dest)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    out << std::fixed << std::setprecision(1) << dest.ref << 'd';
    return out;
  }

  std::ostream& operator<<(std::ostream& out, const RationalIO& dest)
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

  std::ostream& operator<<(std::ostream& out, const LabelIO& dest)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    out << ':' << dest.exp << ' ';
    return out;
  }

  std::ostream& operator<<(std::ostream& out, const StringIO& dest)
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
    double key1 = dest.key1;
    std::pair< long long, unsigned long long > key2 = dest.key2;
    std::string key3 = dest.key3;
    out << '(' << LabelIO{ "key1" } << DoubleIO{ key1 };
    out << LabelIO{ "key2" } << RationalIO{ key2 };
    out << LabelIO{ "key3" } << StringIO{ key3 } << ":)";
    return out;
  }

  iofmtguard::iofmtguard(std::basic_ios< char >& s):
    s_(s),
    width_(s.width()),
    fill_(s.fill()),
    precision_(s.precision()),
    fmt_(s.flags())
  {}

  iofmtguard::~iofmtguard()
  {
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
  }
}

