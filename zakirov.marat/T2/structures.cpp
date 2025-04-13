#include "structures.hpp"
#include <iostream>
#include <stack>

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

  in >> MinorSymbol{'0'};
  std::string oct_number;
  char next_number;
  in >> next_number;
  while (next_number >= '0' && next_number < '8')
  {
    oct_number += next_number;
  }

  if (next_number != ':')
  {
    in.setstate(std::ios::failbit);
  }

  try
  {
    object.ref = std::stoi(oct_number, nullptr, 8);
  }
  catch (const std::out_of_range)
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream & zakirov::operator>>(std::istream & in, UllHexIO && object)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  in >> MinorSymbol{'0'};
  
  std::string hex_number;
  char next_number;
  in >> next_number;
  if (next_number != 'x' && next_number != 'X')
  {
    in.setstate(std::ios::failbit);
  }

  while ((next_number >= '0' && next_number <= '9') || (next_number >= 'A' && next_number <= 'F'))
  {
    hex_number += next_number;
  }

  if (next_number != ':')
  {
    in.setstate(std::ios::failbit);
  }

  try
  {
    object.ref = std::stoi(hex_number, nullptr, 16);
  }
  catch (const std::out_of_range)
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream & zakirov::operator>>(std::istream & in, StringIO && object)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  in >> MinorSymbol{'"'};
  std::string string_r;
  char next_symbol;
  in >> next_symbol;
  while (next_symbol != '"')
  {
    string_r += next_symbol;
  }

  in >> MinorSymbol{':'};
  object.exp = string_r;
  return in;
}
