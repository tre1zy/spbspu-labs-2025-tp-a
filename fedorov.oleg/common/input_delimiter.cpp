#include "input_delimiter.hpp"

#include <cctype>

std::istream &fedorov::operator>>(std::istream &in, DelimiterStr &&dest)
{
  std::istream::sentry sentry(in);
  if (sentry && dest.exp.find(in.get()) == dest.exp.npos)
  {
    in.unget();
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream &fedorov::operator>>(std::istream &in, DelimiterInput &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c;
  in.get(c);
  if (in && std::tolower(c) != std::tolower(dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

