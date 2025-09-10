#include "value.hpp"
#include <bitset>
#include <string>
#include "Delimiter.hpp"

std::istream &smirnov::operator>>(std::istream &in, ULLBinary &&key)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    in.setstate(std::ios::failbit);
  }
  std::bitset< 64 > bin;
  in >> DelimiterStringI{ "0b" } >> bin;
  if (in)
  {
    key.value = bin.to_ulong();
  }
  return in;
}

