#include "input-struct.hpp"

namespace aleksandrov
{
  std::istream& operator>>(std::istream& in, DelimeterI&& dest)
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

  std::istream& operator>>(std::istream& in, DoubleI&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return in >> dest.ref >> DelimeterI{ 'd' };
  }

  std::istream& operator>>(std::istream& in, RationalI&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    using sep = DelimeterI;
    using ll = LongLongI;
    using ull = UnsignedLongLongI;
    in >> sep{ '(' } >> sep{ ':' };
    in >> sep{ 'N' } >> ll{ dest.ref.first };
    in >> sep { ':' };
    in >> sep{ 'D' } >> ull{ dest.ref.second };
    in >> sep{ ':' } >> sep{ ')' };
    return in;
  }

  std::istream& operator>>(std::istream& in, LongLongI&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return in >> dest.ref;
  }

  std::istream& operator>>(std::istream& in, UnsignedLongLongI&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return in >> dest.ref;
  }

  std::istream& operator>>(std::istream& in, StringI&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return std::getline(in >> DelimeterI{ '"' }, dest.ref, '"');
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
      using sep = DelimeterI;
      using dbl = DoubleI;
      using rat = RationalI;
      using str = StringI;
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
}

