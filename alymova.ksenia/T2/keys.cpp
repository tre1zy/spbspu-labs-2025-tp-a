#include "keys.hpp"
#include <iostream>
#include <exception>
#include <iostream>

std::istream& alymova::operator>>(std::istream& in, Delimiter&& object)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char delim;
  if ((in >> delim) && delim != object.exp)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
std::istream& alymova::operator>>(std::istream& in, Label&& object)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::string s;
  if ((in >> s) && s != object.exp)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
std::istream& alymova::operator>>(std::istream& in, UllOct&& object)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> Delimiter{'0'} >> object.i_oct;
  std::string s_oct = std::to_string(object.i_oct);
  if ((s_oct.find('8') != std::string::npos) || (s_oct.find('9') != std::string::npos))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
std::istream& alymova::operator>>(std::istream& in, ChrLit&& object)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return in >> Delimiter{'\''} >> object.c >> Delimiter{'\''};
}
std::istream& alymova::operator>>(std::istream& in, StringIO&& object)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return std::getline(in >> Delimiter{'"'}, object.s, '"');
}
std::istream& alymova::operator>>(std::istream& in, DataStruct& object)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct input;
  in >> Delimiter{'('};
  in >> Delimiter{':'};
  in >> Label{"key1"};
  in >> UllOct{input.key1};
  in >> Delimiter{':'};
  in >> Label{"key2"};
  in >> ChrLit{input.key2};
  in >> Delimiter{':'};
  in >> Label{"key3"};
  in >> StringIO{input.key3};
  in >> Label{":)"};
  if (in)
  {
    object = input;
  }
  return in;
}
std::ostream& alymova::operator<<(std::ostream& out, const DataStruct& object)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << "(";
  out << ":key1 ";
  out << '0' << object.key1;
  out << ":key2 ";
  out << "'" << object.key2 << "'";
  out << ":key3 ";
  out << "\"" << object.key3 << "\"";
  out << ":)";
  return out;
}
alymova::StreamGuard::StreamGuard(std::basic_ios< char >& s):
  s_(s),
  width_(s.width()),
  fill_(s.fill()),
  precision_(s.precision()),
  fmt_(s.flags())
{}
alymova::StreamGuard::~StreamGuard()
{
  s_.width(width_);
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}
