#include "data_struct.hpp"
#include "stream_guard.hpp"
#include "input.hpp"
#include "delimiter.hpp"

bool shak::operator<(const DataStruct &left, const DataStruct &right)
{
  if (left.key1 != right.key1)
  {
    return left.key1 < right.key1;
  }

  else if (left.key2 != right.key2)
  {
    return left.key2 < right.key2;
  }
  else
  {
    return left.key3.size() < right.key3.size();
  }
}

std::istream &shak::operator>>(std::istream &in, DataStruct &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct data;
  using check = DelimiterIO;
  using ch = ChrLit;
  using rat = RatLsp;
  using str = Str;
  in >> check{'('};
  for (size_t i = 0; i < 3; ++i)
  {
    std::string key;
    in >> check{':'} >> key;
    if (key == "key1")
    {
      in >> ch{data.key1};
    }
    else if (key == "key2")
    {
      in >> rat{data.key2};
    }
    else if (key == "key3")
    {
      in >> str{data.key3};
    }
    else
    {
      in.setstate(std::ios::failbit);
      break;
    }
  }
  in >> check{':'} >> check{')'};
  if (in)
  {
    dest = data;
  }
  return in;
}

std::ostream &shak::operator<<(std::ostream &out, const DataStruct &src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  shak::StreamGuard guard(out);
  out << "(";
  out << ":key1 \'" << src.key1 << '\'';
  out << ":key2 (:N " << src.key2.first << ":D " << src.key2.second << ":)";
  out << ":key3 \"" << src.key3 << '"';
  out << ":)";
  return out;
}
