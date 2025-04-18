#include "dataStruct.hpp"

namespace
{
  struct DelimiterIO
  {
    char exp;
  };

  struct UlloctIO
  {
    unsigned long long & ref;
  };

  struct ComplexIO
  {
    std::complex< double > & ref;
  };

  struct StringIO
  {
    std::string & ref;
  };

  struct LabelIO
  {
    std::string exp;
  };

  std::istream & operator>>(std::istream & in, DelimiterIO && dest)
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

  std::istream & operator>>(std::istream & in, UlloctIO && dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    in >> DelimiterIO{ '0' } >> dest.ref;
    auto a = dest.ref;
    while (a > 0)
    {
      if (a % 10 >= 8)
      {
        in.setstate(std::ios::failbit);
      }
      a /= 10;
    }
    return in;
  }

  std::istream & operator>>(std::istream & in, ComplexIO && dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    in >> DelimiterIO{ '#' } >> DelimiterIO{ 'c' };
    double real, imag = 0;
    in >> DelimiterIO{ '(' } >> real;
    in >> imag >> DelimiterIO{ ')' };
    dest.ref.real(real);
    dest.ref.imag(imag);
  }

  std::istream & operator>>(std::istream & in, StringIO && dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
  }

  std::istream & operator>>(std::istream & in, LabelIO && dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    std::string data = "";
    if ((in >> StringIO{ data }) && (data != dest.exp))
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }
}

std::istream & mozhegova::operator>>(std::istream & in, mozhegova::DataStruct & dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
}

std::ostream & mozhegova::operator<<(std::ostream & out, const mozhegova::DataStruct & dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
}

bool mozhegova::DataStruct::operator<(const mozhegova::DataStruct & other) const
{
  if (key1 != other.key1)
  {
    return key1 < other.key1;
  }
  else if (key2 != other.key2)
  {
    return std::abs(key2) < std::abs(other.key2);
  }
  else
  {
    return key3.size() < other.key3.size();
  }
}
