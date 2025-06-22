#include "datastruct.h"
#include <cctype>
#include <sstream>
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
}

std::istream& asafov::operator>>(std::istream& in, DataStruct& data)
{
  std::string line;
  while (std::getline(in, line))
  {
    std::istringstream iss(line);
    DataStruct temp;
    char c;

    if (!(iss >> c) || c != '(') continue;
    if (!(iss >> c) || c != ':') continue;

    std::string key;
    iss >> key;
    if (key != "key1") continue;
    if (!(iss >> c) || c != ' ') continue;

    if (!parseULLBin(iss, temp.key1)) continue;

    if (!(iss >> c) || c != ':') continue;
    iss >> key;
    if (key != "key2") continue;
    if (!(iss >> c) || c != ' ') continue;

    if (!parseCmpLsp(iss, temp.key2)) continue;

    if (!(iss >> c) || c != ':') continue;
    iss >> key;
    if (key != "key3") continue;
    if (!(iss >> c) || c != ' ') continue;

    if (!parseQuotedString(iss, temp.key3)) continue;

    if (!(iss >> c) || c != ':') continue;
    if (!(iss >> c) || c != ')') continue;

    if (iss.peek() != EOF) continue;

    data = temp;
    return in;
  }

  in.setstate(std::ios::failbit);
  return in;
}
