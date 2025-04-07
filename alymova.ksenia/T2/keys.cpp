#include "keys.hpp"
#include <iostream>
#include <exception>
#include <iostream>
#include <sstream>

std::istream& alymova::operator>>(std::istream& in, DelimiterIO&& object)
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
std::istream& alymova::operator>>(std::istream& in, LabelIO&& object)
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
std::istream& alymova::operator>>(std::istream& in, UllOctIO&& object)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterIO{'0'} >> object.i_oct;
  if (in)
  {
    std::string s_oct = std::to_string(object.i_oct);
    if ((s_oct.find('8') != std::string::npos) || (s_oct.find('9') != std::string::npos))
    {
      in.setstate(std::ios::failbit);
    }
  }
  return in;
}
std::istream& alymova::operator>>(std::istream& in, ChrLitIO&& object)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return in >> DelimiterIO{'\''} >> object.c >> DelimiterIO{'\''};
}
std::istream& alymova::operator>>(std::istream& in, StringIO&& object)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return std::getline(in >> DelimiterIO{'"'}, object.s, '"');
}
std::istream& alymova::operator>>(std::istream& in, DataStruct& object)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct input;
  bool flag1 = false;
  bool flag2 = false;
  bool flag3 = false;

  in >> DelimiterIO{'('};
  while ((!flag1 || !flag2 || !flag3) && in)
  {
    std::string name;
    in >> name;
    if (name == ":key1" && !flag1)
    {
      flag1 = true;
      in >> UllOctIO{input.key1};
    }
    else if (name == ":key2" && !flag2)
    {
      flag2 = true;
      in >> ChrLitIO{input.key2};
    }
    else if (name == ":key3" && !flag3)
    {
      flag3 = true;
      in >> StringIO{input.key3};
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
  }
  in >> DelimiterIO{':'};
  in >> DelimiterIO{')'};
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
bool alymova::comparator(const DataStruct& data1, const DataStruct& data2)
{
  if (data1.key1 == data2.key1)
  {
    if (data1.key2 == data2.key2)
    {
      return data1.key3.size() < data2.key3.size();
    }
    return data1.key2 < data2.key2;
  }
  return data1.key1 < data2.key1;
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
