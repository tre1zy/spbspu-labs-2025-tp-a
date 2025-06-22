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
    if (in.peek() != expected) return false;
    in.ignore();
    return true;
  }

  bool parseULLBin(std::istream& in, unsigned long long& result)
  {
    if (!expect(in, '0')) return false;
    if (!expect(in, 'b')) return false;

    result = 0;
    bool hasDigits = false;
    while (in.peek() == '0' || in.peek() == '1')
    {
      result = (result << 1) | (in.get() - '0');
      hasDigits = true;
    }
    return hasDigits;
  }

  bool parseCmpLsp(std::istream& in, std::complex< double >& result)
  {
    if (!expect(in, '#')) return false;
    if (!expect(in, 'c')) return false;
    if (!expect(in, '(')) return false;

    double real, imag;
    in >> real >> imag;
    if (!in) return false;

    if (!expect(in, ')')) return false;

    result = {real, imag};
    return true;
  }

  bool parseQuotedString(std::istream& in, std::string& result)
  {
    if (!expect(in, '"')) return false;

    result.clear();
    char c;
    while (in.get(c) && c != '"')
    {
      result += c;
    }

    return c == '"';
  }
}

std::istream& asafov::operator>>(std::istream& in, DataStruct& data)
{
  DataStruct temp;

  if (!expect(in, '(') || !expect(in, ':'))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::string key1_tag;
  in >> key1_tag;
  if (key1_tag != "key1" || !expect(in, ' '))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (!parseULLBin(in, temp.key1))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (!expect(in, ':') || !(in >> key1_tag) || key1_tag != "key2" || !expect(in, ' '))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (!parseCmpLsp(in, temp.key2))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (!expect(in, ':') || !(in >> key1_tag) || key1_tag != "key3" || !expect(in, ' '))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (!parseQuotedString(in, temp.key3))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (!expect(in, ':') || !expect(in, ')'))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  data = temp;
  return in;
}
