#include "datastruct.h"
#include <cctype>

namespace
{
  void skipWhitespace(std::istream& in)
  {
    while (std::isspace(in.peek()))
        {
            in.ignore();
        }
  }

  bool expect(std::istream& in, char expected)
  {
    skipWhitespace(in);
    if (in.peek() != expected)
    {
      in.setstate(std::ios::failbit);
      return false;
    }
    in.ignore();
    return true;
  }

  unsigned long long parseULLBin(std::istream& in)
  {
    if (!expect(in, '0'))
    {
      return 0;
    }
    if (!expect(in, 'b'))
    {
      return 0;
    }

    unsigned long long result = 0;
    while (in.peek() == '0' || in.peek() == '1')
    {
      result = (result << 1) | (in.get() - '0');
    }
    return result;
  }

  std::complex<double> parseCmpLsp(std::istream& in)
  {
    if (!expect(in, '#'))
    {
      return {0.0, 0.0};
    }
    if (!expect(in, 'c'))
    {
      return {0.0, 0.0};
    }
    if (!expect(in, '('))
    {
      return {0.0, 0.0};
    }

    double real = 0.0;
    in >> real;

    double imag = 0.0;
    in >> imag;

    if (!expect(in, ')'))
    {
      return {0.0, 0.0};
    }

    return {real, imag};
  }

  std::string parseQuotedString(std::istream& in)
  {
    if (!expect(in, '"'))
    {
      return "";
    }

    std::string result;
    char c;
    while (in.get(c) && c != '"')
    {
      result += c;
    }

    if (c != '"')
    {
      in.setstate(std::ios::failbit);
      return "";
    }

    return result;
  }
}

std::istream& asafov::operator>>(std::istream& in, DataStruct& data)
{
  DataStruct temp;

  if (!expect(in, '('))
  {
    return in;
  }
  if (!expect(in, ':'))
  {
    return in;
  }

  std::string key1_tag;
  in >> key1_tag;
  if (key1_tag != "key1" || !expect(in, ' '))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  temp.key1 = parseULLBin(in);
  if (temp.key1 == 0 && in.peek() != '0')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (!expect(in, ':'))
  {
    return in;
  }
  std::string key2_tag;
  in >> key2_tag;
  if (key2_tag != "key2" || !expect(in, ' '))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  temp.key2 = parseCmpLsp(in);
  if (temp.key2 == std::complex<double>{0.0, 0.0})
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (!expect(in, ':'))
  {
    return in;
  }
  std::string key3_tag;
  in >> key3_tag;
  if (key3_tag != "key3" || !expect(in, ' '))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  temp.key3 = parseQuotedString(in);
  if (temp.key3.empty())
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (!expect(in, ':'))
  {
    return in;
  }
  if (!expect(in, ')'))
  {
    return in;
  }

  data = temp;
  return in;
}