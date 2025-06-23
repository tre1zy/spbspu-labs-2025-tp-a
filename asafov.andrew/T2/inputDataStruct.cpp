#include "datastruct.h"
#include <cctype>
#include <algorithm>

namespace
{
  unsigned long long parseULLBin(std::istream& is)
  {
    char ch;
    if (!is.get(ch) || ch != '0')
    {
      is.setstate(std::ios::failbit);
      return 0;
    }
    if (!is.get(ch) || ch != 'b')
    {
      is.setstate(std::ios::failbit);
      return 0;
    }

    unsigned long long result = 0;
    std::string bits;
    while (is.get(ch) && (ch == '0' || ch == '1'))
    {
      bits.push_back(ch);
    }
    if (bits.empty())
    {
      is.setstate(std::ios::failbit);
      return 0;
    }
    if (is)
    {
      is.unget();
    }

    for (char bit: bits)
    {
      result = (result << 1) | (bit == '1');
    }
    return result;
  }

  std::complex< double > parseCmpLsp(std::istream& is)
  {
    char ch;
    if (!is.get(ch) || ch != '#')
    {
      is.setstate(std::ios::failbit);
      return {0.0, 0.0};
    }
    if (!is.get(ch) || ch != 'c')
    {
      is.setstate(std::ios::failbit);
      return {0.0, 0.0};
    }
    if (!is.get(ch) || ch != '(')
    {
      is.setstate(std::ios::failbit);
      return {0.0, 0.0};
    }

    double real = 0.0;
    double imag = 0.0;
    is >> real;
    if (!is)
    {
      return {0.0, 0.0};
    }

    if (is.get(ch) && ch != ')')
    {
      is.unget();
      is >> imag;
      if (!is)
      {
        return {0.0, 0.0};
      }
      if (!is.get(ch) || ch != ')')
      {
        is.setstate(std::ios::failbit);
        return {0.0, 0.0};
      }
    }
    return {real, imag};
  }
}

std::istream& asafov::operator>>(std::istream& is, DataStruct& data)
{
  DataStruct temp;
  std::string line;
  if (!std::getline(is, line))
  {
    return is;
  }

  size_t pos = 0;
  bool has_key1 = false;
  bool has_key2 = false;
  bool has_key3 = false;

  while (pos < line.length())
  {
    if (line[pos] == ':')
    {
      size_t key_start = pos + 1;
      size_t key_end = line.find_first_of(" :", key_start);
      if (key_end == std::string::npos)
      {
        break;
      }

      std::string key = line.substr(key_start, key_end - key_start);
      pos = line.find_first_not_of(" ", key_end);

      if (key == "key1" && pos != std::string::npos)
      {
        size_t val_end = line.find(':', pos);
        std::string val_str = line.substr(pos, val_end - pos);
        std::istringstream iss(val_str);
        temp.key1 = parseULLBin(iss);
        if (!iss.fail() || val_str == "0b0")
        {
          has_key1 = true;
          pos = val_end;
        }
      }
      else if (key == "key2" && pos != std::string::npos)
      {
        size_t val_end = line.find(':', pos);
        std::string val_str = line.substr(pos, val_end - pos);
        std::istringstream iss(val_str);
        temp.key2 = parseCmpLsp(iss);
        if (!iss.fail())
        {
          has_key2 = true;
          pos = val_end;
        }
      }
      else if (key == "key3" && pos != std::string::npos && line[pos] == '"')
      {
        pos++;
        size_t quote_end = line.find('"', pos);
        if (quote_end != std::string::npos)
        {
          temp.key3 = line.substr(pos, quote_end - pos);
          has_key3 = true;
          pos = quote_end + 1;
        }
      }
    }
    else
    {
      pos++;
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
