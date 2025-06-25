#include "input_delimiter.hpp"

#include <cctype>

std::istream &fedorov::operator>>(std::istream &in, DelimiterStr &&exp)
{
  std::istream::sentry guard(in);
  if (!guard)
  {
    return in;
  }
  for (size_t i = 0; exp.exp[i] != '\0'; ++i)
  {
    in >> DelimiterInput{exp.exp[i]};
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

