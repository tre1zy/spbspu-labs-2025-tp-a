#include "datastruct.h"
#include <cctype>
#include <complex>
#include <iterator>
#include <string>

namespace
{
  using It = std::istreambuf_iterator<char>;

  void skipWhitespace(It& it, const It& end)
  {
    while (it != end && std::isspace(*it))
      ++it;
  }

  bool matchPrefix(It& it, const It& end, const std::string& prefix)
  {
    It temp = it;
    for (char ch : prefix)
    {
      if (temp == end || *temp != ch)
        return false;
      ++temp;
    }
    it = temp;
    return true;
  }

  std::string parseIdentifier(It& it, const It& end)
  {
    std::string result;
    while (it != end && std::isalnum(*it))
    {
      result += *it;
      ++it;
    }
    return result;
  }

  unsigned long long parseBinary(It& it, const It& end)
  {
    if (!matchPrefix(it, end, "0b"))
      throw std::ios_base::failure("Expected binary prefix");

    unsigned long long result = 0;
    bool hasDigits = false;

    while (it != end && (*it == '0' || *it == '1'))
    {
      result = (result << 1) | (*it - '0');
      ++it;
      hasDigits = true;
    }

    if (!hasDigits)
      throw std::ios_base::failure("Empty binary number");

    return result;
  }

  std::complex<double> parseComplex(It& it, const It& end)
  {
    if (!matchPrefix(it, end, "#c("))
      throw std::ios_base::failure("Expected #c(");

    std::string number;
    skipWhitespace(it, end);
    while (it != end && (std::isdigit(*it) || *it == '.' || *it == '-' || *it == '+'))
    {
      number += *it;
      ++it;
    }

    double real = std::stod(number);
    skipWhitespace(it, end);

    number.clear();
    while (it != end && (std::isdigit(*it) || *it == '.' || *it == '-' || *it == '+'))
    {
      number += *it;
      ++it;
    }

    double imag = std::stod(number);
    skipWhitespace(it, end);

    if (it == end || *it != ')')
      throw std::ios_base::failure("Expected ')' in complex");
    ++it;

    return {real, imag};
  }

  std::string parseQuotedString(It& it, const It& end)
  {
    if (it == end || *it != '"')
      throw std::ios_base::failure("Expected opening quote");

    ++it;
    std::string result;
    while (it != end && *it != '"')
    {
      result += *it;
      ++it;
    }

    if (it == end || *it != '"')
      throw std::ios_base::failure("Unterminated string");
    ++it;

    return result;
  }
}

std::istream& asafov::operator>>(std::istream& is, DataStruct& data)
{
  std::istreambuf_iterator<char> it(is), end;
  DataStruct temp;
  bool has_key1 = false, has_key2 = false, has_key3 = false;

  try
  {
    while (it != end)
    {
      skipWhitespace(it, end);
      if (it == end || *it != ':')
        break;
      ++it;

      std::string key = parseIdentifier(it, end);
      skipWhitespace(it, end);

      if (key == "key1")
      {
        temp.key1 = parseBinary(it, end);
        has_key1 = true;
      }
      else if (key == "key2")
      {
        temp.key2 = parseComplex(it, end);
        has_key2 = true;
      }
      else if (key == "key3")
      {
        temp.key3 = parseQuotedString(it, end);
        has_key3 = true;
      }
      else
      {
        throw std::ios_base::failure("Unknown key");
      }

      if (has_key1 && has_key2 && has_key3)
      {
        data = temp;
        return is;
      }
    }

    throw std::ios_base::failure("Missing required keys");
  }
  catch (...)
  {
    is.setstate(std::ios::failbit);
    return is;
  }
}
