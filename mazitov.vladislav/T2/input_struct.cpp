#include "input_struct.hpp"
#include "stream_guard.hpp"

std::istream& mazitov::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '\0';
  in >> c;
  if (in && (c != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& mazitov::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}

std::istream& mazitov::operator>>(std::istream& in, DoubleIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> dest.ref;
  if (!in)
  {
    return in;
  }

  char literal;
  in >> literal;
  if (literal != 'd' && literal != 'D')
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& mazitov::operator>>(std::istream& in, UnsignedLongLongBinIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  in >> DelimiterIO{ '0' } >> DelimiterIO{ 'b' };

  char c = 0;
  std::string binaryStr;
  while (in >> c && (c == '0' || c == '1'))
  {
    binaryStr += c;
  }

  if (binaryStr.empty())
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  unsigned long long binaryValue = 0;
  for (char bit : binaryStr)
  {
    binaryValue = (binaryValue << 1) | (bit == '1' ? 1 : 0);
  }
  dest.ref = binaryValue;
  return in;
}

