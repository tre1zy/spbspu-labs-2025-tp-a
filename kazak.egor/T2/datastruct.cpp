#include "datastruct.hpp"
#include "stream_guard.hpp"

bool kazak::operator<(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 != rhs.key1)
  {
    return lhs.key1 < rhs.key1;
  }
  if (lhs.key2 != rhs.key2)
  {
    return lhs.key2 < rhs.key2;
  }
  return lhs.key3.size() < rhs.key3.size();
}

std::istream& kazak::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c = '0';
  in >> c;

  if (in && (c != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream& kazak::operator>>(std::istream& in, ULLHexIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  in >> std::hex >> dest.ref;
  if (!in)
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream& kazak::operator>>(std::istream& in, RationalIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  long long numerator = 0;
  unsigned long long denominator = 0;

  in >> DelimiterIO{'('} >> LabelIO{":N"} >> numerator;
  in >> LabelIO{":D"} >> denominator;
  in >> DelimiterIO{':'} >> DelimiterIO{')'};

  if (in)
  {
    dest.ref = std::make_pair(numerator, denominator);
  }

  return in;
}

std::istream& kazak::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  return std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
}

std::istream& kazak::operator>>(std::istream& in, LabelIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  std::string data;
  if ((in >> data) && (data != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream& kazak::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  DataStruct input;
  bool hasKey1 = false;
  bool hasKey2 = false;
  bool hasKey3 = false;

  in >> DelimiterIO{'('};

  std::string key;
  while (in >> DelimiterIO{':'} && in >> key)
  {
    if (key == "key1")
    {
      in >> ULLHexIO{input.key1};
      hasKey1 = true;
    }
    else if (key == "key2")
    {
      in >> RationalIO{input.key2};
      hasKey2 = true;
    }
    else if (key == "key3")
    {
      in >> StringIO{input.key3};
      hasKey3 = true;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
  }

  in >> DelimiterIO{')'};

  if (hasKey1 && hasKey2 && hasKey3)
  {
    dest = std::move(input);
  }
  else
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::ostream& kazak::operator<<(std::ostream& out, const DataStruct& src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  StreamGuard guard(out);
  out << "(:key1 0x" << std::hex << std::uppercase << src.key1;
  out << ":key2 (:N " << std::dec << src.key2.first << ":D ";
  out << src.key2.second << ":)";
  out << ":key3 \"" << src.key3 << "\":)";

  return out;
}
