#include "DataStruct.hpp"
#include <stdexcept>
#include <cmath>
#include <string>
#include <iomanip>
#include <iostream>

namespace ageev
{
  std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char c = '0';
    in >> c;
    if (in && (c != dest.exp))
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
    return in >> dest.ref >> DelimiterIO{ 'd' };
  }

  std::istream& operator>>(std::istream& in, DoubleSciIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char c = '0';
    std::string str;
    try
    {
      while (in >> c && c != 'e' && c != 'E')
      {
        str += c;
      }
      double number = std::stod(str);
      str.clear();
      while (in >> c && c != ':')
      {
        str += c;
      }
      int degree = std::stoi(str);
      dest.ref = number * std::pow(10, degree);
    }
    catch (const std::exception& e)
    {
      in.setstate(std::ios::failbit);
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
    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
  }

  std::istream& operator>>(std::istream& in, LabelIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
      std::string data;
    in >> data;
    if (data != dest.exp) {
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
    bool hasKey1 = false, hasKey2 = false, hasKey3 = false;
    int fields = 0;

    using sep = DelimiterIO;
    using dbl = DoubleIO;
    using dblsci = DoubleSciIO;
    using str = StringIO;

    in >> sep{'('};
    while (fields < 3 && in)
    {
      std::string key;
      in >> sep{':'} >> key;
      if (key == "key1" && !hasKey1)
      {
        if (in >> dbl{input.key1}) hasKey1 = true;
        else break;
      }
      else if (key == "key2" && !hasKey2)
      {
        if (in >> dblsci{input.key2}) hasKey2 = true;
        else break;
      }
      else if (key == "key3" && !hasKey3)
      {
        if (in >> str{input.key3}) hasKey3 = true;
        else break;
      }
      else
      {
        in.setstate(std::ios::failbit);
        break;
      }

      fields++;
    }
    in >> sep{':'} >> sep{')'};
    if (hasKey1 && hasKey2 && hasKey3)
    {
      dest = input;
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
    iofmtguard fmtguard(out);
    out << "(:key1 " << std::fixed << std::setprecision(1)
        << src.key1 << "d:key2 " << std::defaultfloat
        << src.key2 << ":key3 \"" << src.key3 << "\":)";
    return out;
  }

  iofmtguard::iofmtguard(std::basic_ios< char >& s) :
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
