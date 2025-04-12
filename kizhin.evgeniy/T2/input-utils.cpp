#include "input-utils.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>
#include "stream-guard.hpp"

namespace kizhin {
  std::istream& inputKey(std::istream&, const std::string&, DataStruct&);
}

std::istream& kizhin::detail::input::operator>>(std::istream& in, Delimiter&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  char c = 0;
  if ((in >> c) && (c != dest.val)) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& kizhin::detail::input::operator>>(std::istream& in, OneOfDelimiters&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  char c = 0;
  const std::string& exp = dest.val;
  if ((in >> c) && std::find(exp.begin(), exp.end(), c) == exp.end()) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& kizhin::detail::input::operator>>(std::istream& in, Nominator&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return in >> Delimiter{ 'N' } >> dest.val;
}

std::istream& kizhin::detail::input::operator>>(std::istream& in, Denominator&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return in >> Delimiter{ 'D' } >> dest.val;
}

std::istream& kizhin::detail::input::operator>>(std::istream& in, Rational&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  in >> Delimiter{ '(' } >> Delimiter{ ':' };
  in >> Nominator{ dest.val.first };
  in >> Delimiter{ ':' };
  in >> Denominator{ dest.val.second };
  in >> Delimiter{ ':' } >> Delimiter{ ')' };
  return in;
}

std::istream& kizhin::detail::input::operator>>(std::istream& in, Double&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return in >> dest.val >> OneOfDelimiters{ "dD" };
}

std::istream& kizhin::detail::input::operator>>(std::istream& in, String&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return std::getline(in >> Delimiter{ '"' }, dest.val, '"');
}

std::istream& kizhin::detail::input::operator>>(std::istream& in, Label&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  std::string data;
  if (std::getline(in, data, ' ') && (data != dest.val)) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& kizhin::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  StreamGuard guard(in);
  in >> std::skipws;
  using namespace detail::input;
  in >> Delimiter{ '(' } >> Delimiter{ ':' };
  std::vector< std::string > keys{ "key1", "key2", "key3" };
  DataStruct input;
  for (std::string key; !keys.empty() && std::getline(in, key, ' ');) {
    keys.erase(std::find(keys.begin(), keys.end(), key));
    inputKey(in, key, input);
    in >> Delimiter{ ':' };
  }
  if ((in >> Delimiter{ ')' }) && keys.empty()) {
    dest = input;
  }
  return in;
}

std::istream& kizhin::inputKey(std::istream& in, const std::string& key, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  using namespace detail::input;
  if (key == "key1") {
    return in >> Double{ dest.key1 };
  } else if (key == "key2") {
    return in >> Rational{ dest.key2 };
  } else if (key == "key3") {
    return in >> String{ dest.key3 };
  }
  in.setstate(std::ios::failbit);
  return in;
}

