#include "datastruct.h"
#include <cctype>
#include <algorithm>
#include <iterator>

namespace
{
  unsigned long long parseULLBin(std::istream& is)
  {
    std::istream_iterator<char> it(is);
    std::istream_iterator<char> end;

    if (it == end || *it != '0')
    {
      is.setstate(std::ios::failbit);
      return 0;
    }
    ++it;
    if (it == end || *it != 'b')
    {
      is.setstate(std::ios::failbit);
      return 0;
    }
    ++it;

    unsigned long long result = 0;
    bool hasBits = false;
    while (it != end && (*it == '0' || *it == '1'))
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
    if (it != end)
    {
      is.putback(*it);
    }
    return result;
  }

  std::complex<double> parseCmpLsp(std::istream& is)
  {
    std::istream_iterator<char> it(is);
    std::istream_iterator<char> end;

    if (it == end || *it != '#')
    {
      is.setstate(std::ios::failbit);
      return {0.0, 0.0};
    }
    ++it;
    if (it == end || *it != 'c')
    {
      is.setstate(std::ios::failbit);
      return {0.0, 0.0};
    }
    ++it;
    if (it == end || *it != '(')
    {
      is.setstate(std::ios::failbit);
      return {0.0, 0.0};
    }
    ++it;

    // Parse real part
    std::string realStr;
    while (it != end && ((*it >= '0' && *it <= '9') || *it == '.' || *it == '-' || *it == '+'))
    {
      realStr.push_back(*it);
      ++it;
    }
    if (realStr.empty())
    {
      is.setstate(std::ios::failbit);
      return {0.0, 0.0};
    }

    double real = std::stod(realStr);
    double imag = 0.0;

    // Check if there's an imaginary part
    if (it != end && *it != ')')
    {
      // Parse imaginary part
      std::string imagStr;
      while (it != end && ((*it >= '0' && *it <= '9') || *it == '.' || *it == '-' || *it == '+'))
      {
        imagStr.push_back(*it);
        ++it;
      }
      if (imagStr.empty())
      {
        is.setstate(std::ios::failbit);
        return {0.0, 0.0};
      }
      imag = std::stod(imagStr);
    }

    if (it == end || *it != ')')
    {
      is.setstate(std::ios::failbit);
      return {0.0, 0.0};
    }
    return {real, imag};
  }

  void skipWhitespace(std::istream& is)
  {
    while (is.peek() == ' ' || is.peek() == '\t') {
      is.get();
    }
  }
}

std::istream& asafov::operator>>(std::istream& is, DataStruct& data)
{
  DataStruct temp;
  bool has_key1 = false;
  bool has_key2 = false;
  bool has_key3 = false;

  std::istream_iterator<char> it(is);
  std::istream_iterator<char> end;

  while (it != end)
  {
    if (*it == ':')
    {
      ++it;
      std::string key;
      while (it != end && *it != ' ' && *it != '\n' && *it != ':')
      {
        key.push_back(*it);
        ++it;
      }

      if (key == "key1")
      {
        skipWhitespace(is);
        unsigned long long val = parseULLBin(is);
        if (!is.fail())
        {
          temp.key1 = val;
          has_key1 = true;
        }
      }
      else if (key == "key2")
      {
        skipWhitespace(is);
        std::complex<double> val = parseCmpLsp(is);
        if (!is.fail())
        {
          temp.key2 = val;
          has_key2 = true;
        }
      }
      else if (key == "key3")
      {
        skipWhitespace(is);
        it = std::istream_iterator<char>(is);
        if (it != end && *it == '"')
        {
          ++it;
          std::string str;
          while (it != end && *it != '"' && *it != '\n')
          {
            str.push_back(*it);
            ++it;
          }
          if (it != end && *it == '"')
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

    // Check if we have all keys
    if (has_key1 && has_key2 && has_key3)
    {
      data = temp;
      return is;
    }
  }

  is.setstate(std::ios::failbit);
  return is;
}
