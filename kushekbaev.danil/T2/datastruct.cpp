#include "datastruct.hpp"
#include <limits>
#include <bitset>
#include <iostream>

using sep = kushekbaev::DelimiterIO;
using ull = kushekbaev::ULLIO;
using chr = kushekbaev::CharIO;
using str = kushekbaev::StringIO;

bool kushekbaev::DataStruct::operator<(const DataStruct& other) const
{
  if (key1 != other.key1)
  {
    return key1 < other.key1;
  }
  else if (key2 != other.key2)
  {
    return key2 < other.key2;
  }
  else
  {
    return key3.size() < other.key3.size();
  }
}

std::istream& kushekbaev::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  bool key1exist = false;
  bool key2exist = false;
  bool key3exist = false;
  DataStruct input;
  in >> sep{ '{' };
  for (size_t i = 0; i < 3; ++i)
  {
    std::string label;
    in >> sep{ ':' } >> label;
    if (label == "key1")
    {
      in >> ull{ input.key1 };
      key1exist = true;
    }
    else if (label == "key2")
    {
      in >> chr{ input.key2 };
      key2exist = true;
    }
    else if (label == "key3")
    {
      in >> str{ input.key3 };
      key3exist = true;
    }
    else
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }
  if (!key1exist || !key2exist || !key3exist)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  in >> sep{ ':' } >> sep{ '}' };
  dest = input;
  return in;
}

std::ostream& kushekbaev::operator<<(std::ostream& out, const DataStruct& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << "(";
  out << ":key1 0b" << dest.key1 << ":";
  out << ":key2 '" << dest.key2 << "':";
  out << ":key3 \"" << dest.key3 << "\":";
  out << ")";
  return out;
}

std::istream& kushekbaev::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char delim;
  if ((in >> delim) && delim != dest.exp)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& kushekbaev::operator>>(std::istream& in, ULLIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> sep{ '0' } >> sep{ ( 'b' || 'B' )} >> dest.ref;
  if (in)
  {
    std::string tmp = std::to_string(dest.ref);
    for (char c: tmp)
    {
      if ((c != 1) || (c != 0))
      {
        in.setstate(std::ios::failbit);
        break;
      }
    }
    return in;
  }
  return in;
}

std::istream& kushekbaev::operator>>(std::istream& in, CharIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> sep{ '\'' } >> dest.ref >> sep{ '\'' };
  return in;
}

std::istream& kushekbaev::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> sep{ '\"' } >> dest.ref >> sep{ '\"' };
  return in;
}
