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
  std::string s_oct = std::to_string(object.i_oct);
  if ((s_oct.find('8') != std::string::npos) || (s_oct.find('9') != std::string::npos))
  {
    in.setstate(std::ios::failbit);
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
  StreamGuard guard(in);
  DataStruct input;
  std::string keys[3] = {};
  size_t ids[3] = {0, 1, 2};

  in >> std::noskipws;
  in >> DelimiterIO{'('};
  in >> DelimiterIO{':'};
  {
    std::string name;
    getline(in, name, ' ');
    if (name == "key2")
    {
      ids[0] = 1;
    }
    else if (name == "key3")
    {
      ids[0] = 2;
    }
    else if (name != "key1")
    {
      in.setstate(std::ios::failbit);
    }
  }
  std::getline(in, keys[ids[0]], ':');
  {
    std::string name;
    getline(in, name, ' ');
    if (name == "key1" && ids[0] != 0)
    {
      ids[1] = 0;
    }
    else if (name == "key3" && ids[0] != 2)
    {
      ids[1] = 2;
    }
    else if (!(name == "key2" && ids[0] != 1))
    {
      in.setstate(std::ios::failbit);
    }
  }
  std::getline(in, keys[ids[1]], ':');
  {
    std::string name;
    getline(in, name, ' ');
    if (name == "key1" && ids[0] != 0 && ids[1] != 0)
    {
      ids[2] = 0;
    }
    else if (name == "key2" && ids[0] != 1 && ids[1] != 1)
    {
      ids[2] = 1;
    }
    else if (!(name == "key3" && ids[0] != 2 && ids[1] != 2))
    {
      in.setstate(std::ios::failbit);
    }
  }
  std::getline(in, keys[ids[2]], ':');
  in >> DelimiterIO{')'};

  std::istringstream stream(keys[0] + keys[1] + keys[2]);
  stream >> UllOctIO{input.key1};
  stream >> ChrLitIO{input.key2};
  stream >> StringIO{input.key3};
  if (stream.fail())
  {
    in.setstate(std::ios::failbit);
  }
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
