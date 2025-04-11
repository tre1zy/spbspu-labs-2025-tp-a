#include "dataStruct.hpp"
#include "scopeGuard.hpp"
#include <iostream>
#include <istream>
#include <string>
#include <bitset>
//#include <tuple>

std::istream& kostyukov::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (std::tolower(c) != std::tolower(dest.expected)))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& kostyukov::operator>>(std::istream& in, DelimitersIO&& dest)
{
  for (auto symbol : dest.expected)
  {
    in >> DelimiterIO{ symbol };
  }
  return in;
}

std::istream& kostyukov::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}

std::istream& kostyukov::operator>>(std::istream& in, UllIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::string ull = "";
  char prefix;
  in >> DelimiterIO{ '0' } >> prefix;
  size_t base = 0;
  if (std::tolower(prefix) == 'b')
  {
    base = 2;
  }
  else if (std::tolower(prefix) == 'x')
  {
    base = 16;
  }
  else
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  try
  {
    dest.ref = std::stoull(ull, nullptr, base);
  }
  catch(const std::exception&)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& kostyukov::operator>>(std::istream& in, KeyIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimitersIO{ "key" };
  size_t key;
  in >> key;
  switch(key)
  {
    case 1:
      in >> UllIO{ dest.dataStruct.key1 };
      break;
    case 2:
      in >> UllIO{ dest.dataStruct.key2 };
      break;
    case 3:
      in >> StringIO{ dest.dataStruct.key3 };
      break;
    default:
      in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& kostyukov::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct temp;
  {
    in >> DelimitersIO{ "(:" };
    in >> KeyIO{ temp };
    in >> KeyIO{ temp };
    in >> KeyIO{ temp };
    in >> DelimitersIO{ ")" };
  }
  if (in)
  {
    dest = temp;
  }
  return in;
}

std::ostream& kostyukov::operator<<(std::ostream& out, const UllIO&& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  kostyukov::ScopeGuard scopeGrd(out);
  if (dest.format == UllIO::BIN)
  {
    out << "0b" << std::bitset<64>(dest.ref).to_string().substr(std::bitset<64>(dest.ref).to_string().find('1'));
  }
  else
  {
    out << std::showbase << std::hex << dest.ref;
  }
  return out;
}

std::ostream& kostyukov::operator<<(std::ostream& out, const StringIO&& dest)
{
  return out << "\"" << dest.ref << "\"";
}

std::ostream& kostyukov::operator<<(std::ostream& out, const DataStruct& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  unsigned long long ullBin = dest.key1;
  unsigned long long ullHex = dest.key2;
  std::string string = dest.key3;
  out << "(:key1 " << UllIO{ ullHex }
  << ":key2 " << UllIO{ ullHex }
  << ":key3 " << StringIO{ string }
  << ":)";
  return out;
}

bool kostyukov::comparator(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 != rhs.key1)
  {
    return lhs.key1 < rhs.key1;
  }
  else if (lhs.key2 != rhs.key2)
  {
    return lhs.key2 < rhs.key2;
  }
  return lhs.key3.size() < rhs.key3.size();
}
