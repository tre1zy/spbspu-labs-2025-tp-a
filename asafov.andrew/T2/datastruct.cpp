#include "datastruct.h"
#include <stdexcept>
#include <iterator>
namespace
{
  bool safeDoubleLess(double a, double b, double epsilon = 1e-10)
  {
    return (b - a) > epsilon * std::max(std::abs(a), std::abs(b));
  }

  bool isChar(std::string::iterator& iit, const char ch)
  {
    const char temp = *(iit++);
    if (temp == ch)return true;
    if (temp == ' ') return isChar(iit, ch);
    return false;
  }

  bool isString(std::string::iterator& iit, const char* str)
  {
    for (size_t i = 0; i != '\0'; ++i)
    {
      if (isChar(iit, str[i])) return false;
      ++iit;
    }
    return true;
  }

  unsigned long long readUllBin(std::string::iterator& iit)
  {;
    unsigned long long temp = 0;
    isString(iit, "0b");
    while (*iit == '0' || *iit == '1')
    {
      temp = temp << 1;
      temp += *iit - '0';
    }
    return temp;
  }

  double readDouble(std::string::iterator& iit)
  {
    unsigned long long temp1 = 0;
    double temp2 = 0;
    while (*iit >= '0' && *iit <= '9') temp1 = temp1 * 10 + *(iit++) - '0';
    ++iit;
    while (*iit >= '0' && *iit <= '9') temp2 = temp2 * 10 + *(iit++) - '0';
    while (temp2 > 1.0) temp2 /= 10.0;
    return temp2 + static_cast<double>(temp1);
  }

  void writeString(std::ostream_iterator<char>& oit, const char* str)
  {
    for (size_t i = 0; str[i] != '\0'; ++i) *(oit++) = str[i];
  }

  std::complex<double> readCmpLsp(std::string::iterator& iit)
  {
    std::complex<double> temp = 0;
    isString(iit, "#c(");
    temp.real(readDouble(iit));
    temp.imag(readDouble(iit));
    isString(iit, ")");
    return temp;
  }

  std::string readString(std::string::iterator& iit)
  {
    std::string temp;
    auto it = temp.begin();
    while (*iit == ' ') *(it++) = *(iit++);
    return temp;
  }

  std::string writeUllBin(unsigned long long num)
  {
    if (num == 0) return "0b0";
    
    std::string binary;
    while (num > 0)
    {
      binary = (num % 2 ? "1" : "0") + binary;
      num /= 2;
    }
    
    return "0b" + binary;
  }

  std::string writeCmpLsp(std::complex<double> data)
  {
    std::string str;
    str += '#';
    str += 'c';
    str += '(';
    std::string temp = std::to_string(data.real());
    for (const char i : temp) str += i;
    str += ' ';
    temp = std::to_string(data.imag());
    for (const char i : temp) str += i;
    str += ')';
    return str;
  }

  void writeString(std::ostream_iterator<char>& oit, const std::string& str)
  {
    for (const auto a : str) *(oit++) = a;
  }

  void readKey(std::string::iterator& iit, asafov::DataStruct& data, bool* read)
  {
    if (*iit == '1')
    {
      if (read[0] == true) throw std::logic_error("invalid input!");
      read[0] = true;
      data.key1 = readUllBin(iit);
    }
    else if (*iit == '2')
    {
      if (read[1] == true) throw std::logic_error("invalid input!");
      read[1] = true;
      data.key2 = readCmpLsp(iit);
    }
    else if (*iit == '3')
    {
      if (read[2] == true) throw std::logic_error("invalid input!");
      read[2] = true;
      data.key3 = readString(iit);
    }
    else
    {
      std::cerr << *iit;
      throw std::logic_error("invalid input!");
    }
  }
}

bool asafov::cmpDataStruct(const DataStruct &a, const DataStruct &b)
{
  if (a.key1 != b.key1) return a.key1 < b.key1;
  if (a.key2 != b.key2)
  {
    return a.key2.real() < b.key2.real() ||
      (safeDoubleLess(a.key2.real(), b.key2.real()) && safeDoubleLess(a.key2.imag(), b.key2.imag()));
  }
  return a.key3 < b.key3;
}

std::istream& asafov::operator>>(std::istream& is, DataStruct& data)
{
  bool read[] = {false, false, false};
  std::istream_iterator<char> iit(is);
  std::string stream;
  while (!is.eof()) stream += *(iit++);
  auto it = stream.begin();
  isString(it, "(:key");
  readKey(it, data, read);
  isString(it, ":key");
  readKey(it, data, read);
  isString(it, ":key");
  readKey(it, data, read);
  isString(it, ")");
  return is;
}

std::ostream& asafov::operator<<(std::ostream& os, const DataStruct& data)
{
  std::ostream_iterator<char> oit(os);
  writeString(oit, "(:key1 ");
  writeString(oit, writeUllBin(data.key1));
  writeString(oit, ":key2 ");
  writeString(oit, writeCmpLsp(data.key2));
  writeString(oit, ":key3 \"");
  writeString(oit, data.key3);
  writeString(oit, "\":)\n");
  return os;
}
