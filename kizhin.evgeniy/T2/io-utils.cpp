#include "io-utils.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>
#include "stream-guard.hpp"

namespace kizhin {
  std::istream& inputKey(std::istream&, const std::string&, DataStruct&);
}

std::istream& kizhin::detail::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  char c = 0;
  if ((in >> c) && (c != dest.exp)) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& kizhin::detail::operator>>(std::istream& in, MultiDelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  char c = 0;
  const std::string& exp = dest.exp;
  if ((in >> c) && std::find(exp.begin(), exp.end(), c) == exp.end()) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& kizhin::detail::operator>>(std::istream& in, NominatorIo&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return in >> DelimiterIO{ 'N' } >> dest.ref;
}

std::istream& kizhin::detail::operator>>(std::istream& in, DenominatorIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return in >> DelimiterIO{ 'D' } >> dest.ref;
}

std::istream& kizhin::detail::operator>>(std::istream& in, RationalIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };
  in >> NominatorIo{ dest.ref.first };
  in >> DelimiterIO{ ':' };
  in >> DenominatorIO{ dest.ref.second };
  in >> DelimiterIO{ ':' } >> DelimiterIO{ ')' };
  return in;
}

std::istream& kizhin::detail::operator>>(std::istream& in, DoubleIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return in >> dest.ref >> MultiDelimiterIO{ "dD" };
}

std::istream& kizhin::detail::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}

std::istream& kizhin::detail::operator>>(std::istream& in, LabelIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  std::string data;
  if (std::getline(in, data, ' ') && (data != dest.exp)) {
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
  using namespace detail;
  in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };
  std::vector< std::string > keys{ "key1", "key2", "key3" };
  DataStruct input;
  for (std::string key; !keys.empty() && std::getline(in, key, ' ');) {
    keys.erase(std::find(keys.begin(), keys.end(), key));
    inputKey(in, key, input);
    in >> DelimiterIO{ ':' };
  }
  if ((in >> DelimiterIO{ ')' }) && keys.empty()) {
    dest = input;
  }
  return in;
}

std::ostream& kizhin::operator<<(std::ostream& out, const DataStruct& data)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  StreamGuard guard(out);
  out << '(';
  out << ":key1 " << std::fixed << std::setprecision(1) << data.key1 << 'd';
  out << ":key2 (:N " << data.key2.first << ":D " << data.key2.second << ":)";
  out << ":key3 \"" << data.key3 << '"';
  out << ":)";
  return out;
}

std::istream& kizhin::inputKey(std::istream& in, const std::string& key, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  using namespace detail;
  if (key == "key1") {
    return in >> DoubleIO{ dest.key1 };
  } else if (key == "key2") {
    return in >> RationalIO{ dest.key2 };
  } else if (key == "key3") {
    return in >> StringIO{ dest.key3 };
  }
  in.setstate(std::ios::failbit);
  return in;
}

