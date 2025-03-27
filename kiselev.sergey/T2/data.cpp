#include "data.hpp"
#include <cstddef>
#include <exception>
#include <ios>
#include <iostream>
#include <string>
#include <type_traits>
#include "scopeGuard.hpp"

std::istream& kiselev::operator>>(std::istream& input, DelimeterIO&& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  char c = '0';
  input >> c;
  if (input && (c != dest.exp))
  {
    input.setstate(std::ios::failbit);
  }
  return input;
}

std::istream& kiselev::operator>>(std::istream& input, DelimetersIO&& dest)
{
  for (char c: dest.exp)
  {
    input >> DelimeterIO{ c };
  }
  return input;
}

std::istream& kiselev::operator>>(std::istream& input, UllIO&& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  std::string number;
  input >> number;
  try
  {
    if (number.empty() || number.front() != 0)
    {
      input.setstate(std::ios::failbit);
      return input;
    }
    dest.ref = std::stoull(number, nullptr, 8);
  }
  catch (const std::exception&)
  {
    input.setstate(std::ios::failbit);
  }
  return input;
}

std::istream& kiselev::operator>>(std::istream& input, CharIO&& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  std::string symbol;
  std::getline(input >> DelimeterIO{ '\'' }, symbol, '\'');
  if (symbol.size() != 1)
  {
    input.setstate(std::ios::failbit);
  }
  else
  {
    dest.ref = symbol.front();
  }
  return input;
}

std::istream& kiselev::operator>>(std::istream& input, StringIO&& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  return std::getline(input >> DelimeterIO{ '"' }, dest.ref, '"');
}

std::istream& kiselev::operator>>(std::istream& input, KeyIO&& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  input >> DelimetersIO{ ":key" };
  input >> dest.key;
  switch (dest.key)
  {
  case 1:
    input >> UllIO{ dest.data.key1 };
  case 2:
    input >> CharIO{ dest.data.key2 };
  case 3:
    input >> StringIO{ dest.data.key3 };
  default:
    input.setstate(std::ios::failbit);
  }
  return input;
}

std::istream& kiselev::operator>>(std::istream& input, DataStruct& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  detail::ScopeGuard scope(input);
  input >> DelimetersIO{ "(" };
  input >> KeyIO{ dest, 0 };
  input >> KeyIO{ dest, 0 };
  input >> KeyIO{ dest, 0 };
  input >> DelimetersIO{ ":)" };
  return input;
}
std::ostream& kiselev::operator<<(std::ostream& output, CharIO&& dest)
{
  return output << "\'" << dest.ref << "\'";
}

std::ostream& kiselev::operator<<(std::ostream& output, StringIO&& dest)
{
  return output << "\"" << dest.ref << "\"";
}
std::ostream& kiselev::operator<<(std::ostream& output, DataStruct& dest)
{
  std::ostream::sentry sentry(output);
  if (!sentry)
  {
    return output;
  }
  detail::ScopeGuard scope(output);
  output << "(:key1 " << dest.key1;
  output << ":key2 " << CharIO{ dest.key2 };
  output << ":key3 " << StringIO{ dest.key3 };
  return output;
}
