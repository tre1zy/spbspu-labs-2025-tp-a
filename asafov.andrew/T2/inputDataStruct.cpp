#include "datastruct.h"
#include <iterator>
#include <cctype>
#include <algorithm>
#include <complex>

namespace
{
  bool skipWhitespace(std::istream& is)
  {
    std::istream_iterator<char> it(is);
    while (it != std::istream_iterator<char>() && std::isspace(*it))
    {
      ++it;
    }
    return is.good();
  }

  unsigned long long parseULLBin(std::istream& is)
  {
    std::istream_iterator<char> it(is);
    if (it == std::istream_iterator<char>() || *it != '0') 
    {
      is.setstate(std::ios::failbit);
      return 0;
    }
    ++it;
    if (it == std::istream_iterator<char>() || *it != 'b') 
    {
      is.setstate(std::ios::failbit);
      return 0;
    }
    ++it;

    unsigned long long result = 0;
    bool hasBits = false;
    while (it != std::istream_iterator<char>() && (*it == '0' || *it == '1')) 
    {
      result = (result << 1) | (*it == '1');
      hasBits = true;
      ++it;
    }

    if (!hasBits) 
    {
      is.setstate(std::ios::failbit);
      return 0;
    }
    return result;
  }

  double parseDouble(std::istream& is)
  {
    double result = 0.0;
    bool negative = false;
    bool hasDigits = false;

    std::istream_iterator<char> it(is);
    if (it != std::istream_iterator<char>() && *it == '-') 
    {
      negative = true;
      ++it;
    }

    while (it != std::istream_iterator<char>() && std::isdigit(*it)) 
    {
      result = result * 10 + (*it - '0');
      hasDigits = true;
      ++it;
    }

    if (it != std::istream_iterator<char>() && *it == '.') 
    {
      ++it;
      double fraction = 0.1;
      while (it != std::istream_iterator<char>() && std::isdigit(*it)) 
      {
        result += (*it - '0') * fraction;
        fraction *= 0.1;
        hasDigits = true;
        ++it;
      }
    }

    if (!hasDigits) 
    {
      is.setstate(std::ios::failbit);
      return 0.0;
    }

    return negative ? -result : result;
  }

  std::complex<double> parseCmpLsp(std::istream& is)
  {
    std::istream_iterator<char> it(is);
    if (it == std::istream_iterator<char>() || *it != '#')
    {
      return {0.0, 0.0};
    }
   
    ++it;
    if (it == std::istream_iterator<char>() || *it != 'c')
    {
      return {0.0, 0.0};
    }    
    ++it;
    if (it == std::istream_iterator<char>() || *it != '(')
    {
      return {0.0, 0.0};
    }    
    ++it;

    skipWhitespace(is);
    double real = parseDouble(is);
    skipWhitespace(is);

    double imag = 0.0;
    if (is.peek() != ')') 
    {
      imag = parseDouble(is);
      skipWhitespace(is);
    }

    it = std::istream_iterator<char>(is);
    if (it == std::istream_iterator<char>() || *it != ')')
    {
      return {0.0, 0.0};
    }
    return {real, imag};
  }
}

std::istream& asafov::operator>>(std::istream& is, DataStruct& data)
{
  DataStruct temp;
  bool has_key1 = false, has_key2 = false, has_key3 = false;

  std::istream_iterator<char> it(is), end;
  while (it != end)
  {
    if (*it == ':')
    {
      ++it;
      std::string key;
      while (it != end && !std::isspace(*it) && *it != ':') 
      {
        key.push_back(*it);
        ++it;
      }

      skipWhitespace(is);

      if (key == "key1")
      {
        temp.key1 = parseULLBin(is);
        has_key1 = !is.fail();
      }
      else if (key == "key2")
      {
        temp.key2 = parseCmpLsp(is);
        has_key2 = !is.fail();
      }
      else if (key == "key3")
      {
        if (is.peek() == '"')
        {
          is.get();
          std::string str;
          while (is.peek() != '"' && is.peek() != EOF)
          {
            str.push_back(is.get());
          }
          if (is.get() == '"') 
          {
            temp.key3 = str;
            has_key3 = true;
          }
        }
      }
    }
    else
    {
      ++it;
    }
  }

  if (has_key1 && has_key2 && has_key3)
  {
    data = temp;
  }
  else
  {
    is.setstate(std::ios::failbit);
  }

  return is;
}