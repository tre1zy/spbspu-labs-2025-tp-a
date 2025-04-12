#include "data_struct.hpp"

#include <istream>
#include <utility>
#include "stream_guard.hpp"
#include "skip_any_of.hpp"

template<>
std::istream& rychkov::input::operator>> < rychkov::input::science_literal::value_type,
      rychkov::input::science_literal::id >(std::istream& in, science_literal&& link)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> link.link;
  return in;
}
template<>
std::istream& rychkov::input::operator>> < rychkov::input::ull_literal::value_type,
      rychkov::input::ull_literal::id >(std::istream& in, ull_literal&& link)
{

  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return in >> link.link >> anyOf("LL", "ll");
}
template<>
std::istream& rychkov::input::operator>> < rychkov::input::string_literal::value_type,
      rychkov::input::string_literal::id >(std::istream& in, string_literal&& link)
{

  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return std::getline(in >> anyOf("\""), link.link, '"');
}

std::istream& rychkov::input::operator>>(std::istream& in, nth_ds_field link)
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
  if (!sentry || !(in >> input::anyOf("(")))
  {
    return in;
  }
  unsigned char entered = 0b000;
  size_t matched = -1;
  while (entered != 0b111)
  {
    if (!(in >> input::anyOf(&matched, ":key1 ", ":key2 ", ":key3 ")) || (entered & (1 << matched))
          || !(in >> input::nth_ds_field{matched, link}))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    entered |= (1 << matched);
  }
  return in >> input::anyOf(":)");
}
