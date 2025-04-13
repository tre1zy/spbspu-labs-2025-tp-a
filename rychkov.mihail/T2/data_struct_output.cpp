#include "data_struct.hpp"

#include <ostream>
#include <cmath>
#include "ios_guard.hpp"
#include "skip_any_of.hpp"

template<>
std::ostream& rychkov::iofmt::operator<<< rychkov::iofmt::scientific_literal::value_type,
      rychkov::iofmt::scientific_literal::id >(std::ostream& out, const scientific_literal&& wrapper)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  IosGuard guard(out);
  int power = wrapper.link == 0 ? 0 : 1 + static_cast< int >(std::floor(std::log10(std::fabs(wrapper.link))));
  return out << std::fixed << wrapper.link * std::pow(10., -power) << 'e' << std::showpos << power;
}
template<>
std::ostream& rychkov::iofmt::operator<<< rychkov::iofmt::ull_literal::value_type,
      rychkov::iofmt::ull_literal::id >(std::ostream& out, const ull_literal&& wrapper)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  return out << wrapper.link << "ll";
}
template<>
std::ostream& rychkov::iofmt::operator<<< rychkov::iofmt::string_literal::value_type,
      rychkov::iofmt::string_literal::id >(std::ostream& out, const string_literal&& wrapper)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  return out << '"' << wrapper.link << '"';
}
std::ostream& rychkov::operator<<(std::ostream& out, const DataStruct& link)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << "(:key1 " << iofmt::scientific_literal(link.key1) << ":key2 " << iofmt::ull_literal(link.key2);
  return out << ":key3 " << iofmt::string_literal(link.key3) << ":)";
}
