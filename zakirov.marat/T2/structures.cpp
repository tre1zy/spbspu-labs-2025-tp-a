#include "structures.hpp"
#include <iostream>

std::istream & zakirov::operator>>(std::istream & in, Data & object)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  Data received;
  in >> MinorSymbol{'('};
  std::string key;
  in >> key;
  for (size_t i = 0; i < 3; ++i)
  {
    if (key == ":key1")
    {
      in >> UllOctIO{received.key1};
    }
    else if (key == ":key2")
    {
      in >> UllHexIO{received.key2};
    }
    else if (key == ":key3")
    {
      in >> StringIO{received.key3};
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
  }

  in >> MinorSymbol{':'};
  in >> MinorSymbol{')'};
  if (in)
  {
    object = received;
  }
  return in;
}

std::istream & zakirov::operator>>(std::istream & in, MinorSymbol && object)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char symbol;
  if (in >> symbol && symbol == object.symbol)
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream & zakirov::operator>>(std::istream & in, UllOctIO && object)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

}

std::istream & zakirov::operator>>(std::istream & in, UllHexIO && object)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

}

std::istream & zakirov::operator>>(std::istream & in, StringIO && object)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

}
