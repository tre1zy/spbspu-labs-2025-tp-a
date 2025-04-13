#include "structures.hpp"
#include <iostream>
#include <stack>
#include "stream_guardian.hpp"

bool zakirov::Data::operator<(const Data & other)
{
  if (key1 != other.key1)
  {
    return key1 < other.key1;
  }
  else if (key2 != other.key2)
  {
    return key2 < other.key2;
  }

  return key3.size() < key3.size();
}

std::istream & zakirov::operator>>(std::istream & in, Data & data)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  Data received;
  in >> MinorSymbol{'('};
  in >> MinorSymbol{':'};
  std::string key;
  for (size_t i = 0; i < 3; ++i)
  {
    in >> key;
    if (key == "key1")
    {
      in >> UllOctIO{received.key1};
    }
    else if (key == "key2")
    {
      in >> UllHexIO{received.key2};
    }
    else if (key == "key3")
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
    data = received;
  }

  return in;
}

std::istream & zakirov::operator>>(std::istream & in, MinorSymbol && sym)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char symbol;
  if (!(in >> symbol) || symbol != sym.symbol)
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream & zakirov::operator>>(std::istream & in, UllOctIO && num)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  std::string oct_number;
  char next_number;
  in >> next_number;
  while ((next_number >= '0' && next_number < '8') || next_number == 'u' || next_number == 'l')
  {
    oct_number += next_number;
    in >> next_number;
  }

  if (next_number != ':')
  {
    in.setstate(std::ios::failbit);
  }

  try
  {
    num.ref = std::stoi(oct_number, nullptr, 8);
  }
  catch (const std::out_of_range &)
  {
    in.setstate(std::ios::failbit);
  }
  catch (const std::invalid_argument &)
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream & zakirov::operator>>(std::istream & in, UllHexIO && num)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  std::string hex_num;
  char next_num;
  in >> next_num;
  while ((next_num >= '0' && next_num <= '9') || (next_num >= 'A' && next_num <= 'F') ||
            next_num == 'x' || next_num == 'X')
  {
    hex_num += next_num;
    in >> next_num;
  }

  if (next_num != ':')
  {
    in.setstate(std::ios::failbit);
  }

  try
  {
    num.ref = std::stoi(hex_num, nullptr, 16);
  }
  catch (const std::out_of_range &)
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream & zakirov::operator>>(std::istream & in, StringIO && str)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  in >> MinorSymbol{'"'};
  std::string string_r = "";
  char next_symbol;
  in >> next_symbol;
  while (next_symbol != '"')
  {
    string_r += next_symbol;
    in >> next_symbol;
  }

  in >> MinorSymbol{':'};
  str.exp = string_r;
  return in;
}

std::ostream & zakirov::operator<<(std::ostream & out, const Data & src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  Guardian guardian(out);
  out << "{ \"key1\": ";
  out << std::oct << src.key1;
  out << ", \"key2\": ";
  out << std::hex << src.key2;
  out << ", \"key3\": " << src.key3;
  out << " }";
  return out;
}
