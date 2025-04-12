#include "data_struct.hpp"

#include <istream>
#include <utility>
#include "stream_guard.hpp"
#include "skip_any_of.hpp"

std::istream& rychkov::operator>>(std::istream& in, nth_ds_field link)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  switch (link.field_number)
  {
  case 0:
    return in >> link.link.key1;
  case 1:
    return in >> link.link.key2;
  case 2:
    return in >> link.link.key3;
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
          || !(in >> nth_ds_field{matched, link}))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    entered |= (1 << matched);
  }
  return in >> input::anyOf(":)");
}
