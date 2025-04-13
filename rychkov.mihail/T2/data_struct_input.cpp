#include "data_struct.hpp"

#include <istream>
#include <cmath>
#include "ios_guard.hpp"
#include "skip_any_of.hpp"

template<>
std::istream& rychkov::iofmt::operator>> < rychkov::iofmt::science_literal::value_type,
      rychkov::iofmt::science_literal::id >(std::istream& in, science_literal&& link)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  IosGuard guard(in);
  long long integer = 0, fractional = 0, power = 0;
  in >> integer >> std::noskipws >> anyOf(".") >> fractional;
  if (in && (fractional >= 0) && (in >> anyOf("e", "E") >> power))
  {
    long long temp = fractional, fracLen10 = 0;
    for (; temp != 0; temp /= 10, fracLen10++)
    {}
    link.link = (integer + fractional / std::pow(10., fracLen10)) * std::pow(10., power);
    return in;
  }
  in.setstate(std::ios::failbit);
  return in;
}
template<>
std::istream& rychkov::iofmt::operator>> < rychkov::iofmt::ull_literal::value_type,
      rychkov::iofmt::ull_literal::id >(std::istream& in, ull_literal&& link)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return in >> link.link >> anyOf("LL", "ll");
}
template<>
std::istream& rychkov::iofmt::operator>> < rychkov::iofmt::string_literal::value_type,
      rychkov::iofmt::string_literal::id >(std::istream& in, string_literal&& link)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return std::getline(in >> anyOf("\""), link.link, '"');
}

std::istream& rychkov::iofmt::operator>>(std::istream& in, nth_ds_field link)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  switch (link.field_number)
  {
  case 0:
    return in >> science_literal(link.link.key1);
  case 1:
    return in >> ull_literal(link.link.key2);
  case 2:
    return in >> string_literal(link.link.key3);
  }
  in.setstate(std::ios::failbit);
  return in;
}
std::istream& rychkov::operator>>(std::istream& in, DataStruct& link)
{
  std::istream::sentry sentry(in);
  if (!sentry || !(in >> iofmt::anyOf("(")))
  {
    return in;
  }
  unsigned char entered = 0b000;
  size_t matched = -1;
  while (entered != 0b111)
  {
    if (!(in >> iofmt::anyOf(&matched, ":key1 ", ":key2 ", ":key3 ")) || (entered & (1 << matched))
          || !(in >> iofmt::nth_ds_field{matched, link}))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    entered |= (1 << matched);
  }
  return in >> iofmt::anyOf(":)");
}
