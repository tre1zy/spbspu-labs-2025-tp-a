#include "input_delim.hpp"

std::istream &puzikov::operator>>(std::istream &in, DelimiterI &&dest)
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
