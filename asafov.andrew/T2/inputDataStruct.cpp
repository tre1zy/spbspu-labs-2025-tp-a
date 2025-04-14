#include "datastruct.h"
#include <stdexcept>

namespace
{
  std::string trim(const std::string& s)
  {
  size_t start = s.find_first_not_of(" \t\n\r");
  if (start == std::string::npos) return "";
  size_t end = s.find_last_not_of(" \t\n\r");
  return s.substr(start, end - start + 1);
  }

  unsigned long long parseBinaryULL(const std::string& s)
  {
    if (s.size() < 2 || (s[0] != '0' || (s[1] != 'b' && s[1] != 'B')))
    {
      throw std::invalid_argument("Invalid binary format: " + s);
    }
    return std::strtoull(s.c_str() + 2, nullptr, 2);
  }

  std::complex<double> parseComplex(const std::string& s)
  {
    if (s.size() < 5 || s.substr(0, 2) != "#c" || s[2] != '(' || s.back() != ')')
    {
      throw std::logic_error("Invalid complex format: " + s);
    }

    std::string content = s.substr(3, s.size() - 4);
    size_t space_pos = content.find(' ');
    if (space_pos == std::string::npos)
    {
      throw std::logic_error("Invalid complex format (no space): " + s);
    }

    std::string real_str = trim(content.substr(0, space_pos));
    std::string imag_str = trim(content.substr(space_pos + 1));

    double real = std::strtod(real_str.c_str(), nullptr);
    double imag = std::strtod(imag_str.c_str(), nullptr);

    return std::complex<double>(real, imag);
  }

  std::string parseQuotedString(const std::string& s)
  {
    if (s.size() < 2 || s.front() != '"' || s.back() != '"')
    {
      throw std::invalid_argument("Invalid string format: " + s);
    }
    return s.substr(1, s.size() - 2);
  }

  asafov::DataStruct parseDataStruct(const std::string& input)
  {
    asafov::DataStruct result;
    size_t pos = 0;

    while (pos < input.size())
    {
      if (input[pos] != ':')
      {
        pos++;
        continue;
      }

      size_t key_start = pos + 1;
      size_t key_end = input.find_first_of(" \t\n\r", key_start);
      if (key_end == std::string::npos) key_end = input.size();
      std::string key = input.substr(key_start, key_end - key_start);

      pos = key_end;
      while (pos < input.size() && (input[pos] == ' ' || input[pos] == '\t')) pos++;

      size_t value_end = input.find(':', pos);
      if (value_end == std::string::npos) value_end = input.size();
      std::string value = trim(input.substr(pos, value_end - pos));

      if (key == "key1")
      {
        result.key1 = parseBinaryULL(value);
      }
      else if (key == "key2")
      {
        result.key2 = parseComplex(value);
      }
      else if (key == "key3")
      {
        result.key3 = parseQuotedString(value);
      }
      else
      {
        throw std::invalid_argument("Unknown key: " + key);
      }

      pos = value_end;
    }

    return result;
  }
}

std::istream& asafov::operator>>(std::istream& is, DataStruct& data)
{
  std::string line;
  std::getline(is, line);
  line[line.size() - 1] = ' ';
  line[line.size() - 2] = '\n';
  data = parseDataStruct(line);
  return is;
}
