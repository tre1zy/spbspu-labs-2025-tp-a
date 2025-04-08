#include "structs.hpp"
#include <iostream>
#include "stream-guard.hpp"

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
  alymova::StreamGuard guard(in);
  in >> std::noskipws;

  in >> DelimiterIO{'"'};
  char next;
  while (in >> next && next != '"')
  {
    if (next == '\n')
    {
      in.setstate(std::ios_base::failbit);
      break;
    }
    object.s.push_back(next);
  }
  return in;
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
bool alymova::DataStruct::operator<(const DataStruct& other)
{
  if (key1 == other.key1)
  {
    if (key2 == other.key2)
    {
      return key3.size() < other.key3.size();
    }
    return key2 < other.key2;
  }
  return key1 < other.key1;
}
bool alymova::predForStringIO(char c)
  {
    return (c != '\n' && c != '"');
  }
