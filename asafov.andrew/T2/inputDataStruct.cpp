#include "datastruct.h"
#include <cctype>
#include <algorithm>

namespace
{
  unsigned long long parseULLBin(const std::string& str, size_t& pos)
  {
    if (str.substr(pos, 2) != "0b")
    {
      pos = std::string::npos;
      return 0;
    }
    pos += 2;

    unsigned long long result = 0;
    size_t start = pos;
    while (pos < str.size() && (str[pos] == '0' || str[pos] == '1'))
    {
      result = (result << 1) | (str[pos] - '0');
      ++pos;
    }
    if (pos == start)
    {
      pos = std::string::npos;
      return 0;
    }
    return result;
  }

  std::complex<double> parseCmpLsp(const std::string& str, size_t& pos)
  {
    if (str.substr(pos, 3) != "#c(")
    {
      pos = std::string::npos;
      return {0.0, 0.0};
    }
    pos += 3;

    char* end;
    double real = std::strtod(str.c_str() + pos, &end);
    if (end == str.c_str() + pos)
    {
      pos = std::string::npos;
      return {0.0, 0.0};
    }
    pos = end - str.c_str();

    while (pos < str.size() && std::isspace(str[pos])) ++pos;

    double imag = 0.0;
    if (pos < str.size() && str[pos] != ')')
    {
      imag = std::strtod(str.c_str() + pos, &end);
      if (end == str.c_str() + pos)
      {
        pos = std::string::npos;
        return {0.0, 0.0};
      }
      pos = end - str.c_str();
    }

    if (pos >= str.size() || str[pos] != ')')
    {
      pos = std::string::npos;
      return {0.0, 0.0};
    }
    ++pos;
    return {real, imag};
  }

  std::string parseQuotedString(const std::string& str, size_t& pos)
  {
    if (pos >= str.size() || str[pos] != '"')
    {
      pos = std::string::npos;
      return "";
    }
    ++pos;
    std::string result;
    while (pos < str.size() && str[pos] != '"')
    {
      result.push_back(str[pos++]);
    }
    if (pos == str.size() || str[pos] != '"')
    {
      pos = std::string::npos;
      return "";
    }
    ++pos;
    return result;
  }

  void skipWhitespace(const std::string& str, size_t& pos)
  {
    while (pos < str.size() && std::isspace(str[pos])) ++pos;
  }

  bool matchKey(const std::string& str, size_t& pos, const std::string& key)
  {
    if (str.substr(pos, key.size()) == key)
    {
      pos += key.size();
      return true;
    }
    return false;
  }
}

std::istream& asafov::operator>>(std::istream& is, DataStruct& data)
{
  std::string line;
  while (std::getline(is, line))
  {
    size_t pos = 0;
    DataStruct temp;
    bool has_key1 = false;
    bool has_key2 = false;
    bool has_key3 = false;

    while (pos < line.size())
    {
      if (line[pos] == ':')
      {
        ++pos;
        std::string key;
        while (pos < line.size() && !std::isspace(line[pos]) && line[pos] != ':')
        {
          key.push_back(line[pos++]);
        }
        skipWhitespace(line, pos);

        if (key == "key1")
        {
          unsigned long long val = parseULLBin(line, pos);
          if (pos != std::string::npos)
          {
            temp.key1 = val;
            has_key1 = true;
          }
        }
        else if (key == "key2")
        {
          std::complex<double> val = parseCmpLsp(line, pos);
          if (pos != std::string::npos)
          {
            temp.key2 = val;
            has_key2 = true;
          }
        }
        else if (key == "key3")
        {
          std::string str = parseQuotedString(line, pos);
          if (pos != std::string::npos)
          {
            temp.key3 = str;
            has_key3 = true;
          }
        }
      }
      else
      {
        ++pos;
      }
    }

    if (has_key1 && has_key2 && has_key3)
    {
      data = temp;
      return is;
    }
  }

  is.setstate(std::ios::failbit);
  return is;
}
