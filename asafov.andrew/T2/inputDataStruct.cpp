#include "datastruct.h"
#include <cctype>
#include <limits>

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

  bool tryParseRecord(std::istream& in, asafov::DataStruct& data)
  {
    asafov::DataStruct temp;
    std::streampos start = in.tellg();

    if (!expect(in, '(')) return false;
    if (!expect(in, ':')) return false;

    std::string key;
    in >> key;
    if (key != "key1")
    {
      in.seekg(start);
      return false;
    }
    if (!expect(in, ' '))
    {
      in.seekg(start);
      return false;
    }

    if (!parseULLBin(in, temp.key1))
    {
      in.seekg(start);
      return false;
    }

    if (!expect(in, ':'))
    {
      in.seekg(start);
      return false;
    }
    in >> key;
    if (key != "key2")
    {
      in.seekg(start);
      return false;
    }
    if (!expect(in, ' '))
    {
      in.seekg(start);
      return false;
    }

    if (!parseCmpLsp(in, temp.key2))
    {
      in.seekg(start);
      return false;
    }

    if (!expect(in, ':'))
    {
      in.seekg(start);
      return false;
    }
    in >> key;
    if (key != "key3")
    {
      in.seekg(start);
      return false;
    }
    if (!expect(in, ' '))
    {
      in.seekg(start);
      return false;
    }

    if (!parseQuotedString(in, temp.key3))
    {
      in.seekg(start);
      return false;
    }

    if (!expect(in, ':'))
    {
      in.seekg(start);
      return false;
    }
    if (!expect(in, ')'))
    {
      in.seekg(start);
      return false;
    }

    data = temp;
    return true;
  }
}

std::istream& asafov::operator>>(std::istream& in, DataStruct& data)
{
  while (in.peek() != EOF)
  {
    skipWhitespace(in);

    if (tryParseRecord(in, data))
    {
      return in;
    }

    while (in.peek() != '(' && in.peek() != '\n' && in.peek() != EOF)
    {
      in.ignore();
    }

    if (in.peek() == '\n') in.ignore();
  }

  in.setstate(std::ios::failbit);
  return in;
}
