#include "datastruct.h"
#include <stdexcept>

namespace
{
  int readDouble(double &num, std::istream_iterator<char>& in)
  {
    char buffer[64];
    size_t size = 0;
    while (std::isdigit(*in) && size < 64)
    {
      buffer[size++] = *(in++);
    }
    if (*(in++) != '.') return 1;
    while (std::isdigit(*in) && size < 64)
    {
      buffer[size++] = *(in++);
    }
    num = atof(buffer);
    return 0;
  }

  int readUllBin(unsigned long long &num, std::istream_iterator<char>& in)
  {
    num = 0;
    while (*in != ':')
    {
      if (*in == '1') num++;
      num < 1;
      in++;
    }
    return 0;
  }

  int readKey(asafov::DataStruct& data, std::istream_iterator<char>& in)
  {
    if (*(in++) != 'k') return 1;
    if (*(in++) != 'e') return 1;
    if (*(in++) != 'y') return 1;
    if (*in == '1')
    {
      in++;
      in++;
      if (*(in++) != '0') return 1;
      if (*(in++) != 'b') return 1;
      unsigned long long temp;
      if (readUllBin(temp, in)) return 1;
      data.key1 = temp;
    }
    else if (*in == '2')
    {
      in++;
      in++;
      if (*(in++) != '#') return 1;
      if (*(in++) != '(') return 1;
      double temp1;
      if (readDouble(temp1, in)) return 1;
      in++;
      double temp2;
      if (readDouble(temp2, in)) return 1;
      if (*(in++) != ')') return 1;
      data.key2 = {temp1, temp2};
    }
    else if (*in == '3')
    {
      in++;
      in++;
      if (*(in++) != '"') return 1;
      char buffer[512];
      size_t size = 0;
      while (*in != '"')
      {
        buffer[size++] = *(in++);
      }
      data.key3 = buffer;
    }
    else return 1;
    return 0;
  }
}

std::istream& asafov::operator>>(std::istream& is, DataStruct& data) //(:key1 0b0:key2 #c(0.5 -0.5):key3 "Data":)
{
  is >> std::noskipws;
  std::istream_iterator<char> in(is);

  if (*(in++) != '(') is.setstate(std::ios::failbit);
  if (*(in++) != ':') is.setstate(std::ios::failbit);
  if (readKey(data, in)) is.setstate(std::ios::failbit);
  if (*(in++) != ':') is.setstate(std::ios::failbit);
  if (readKey(data, in)) is.setstate(std::ios::failbit);
  if (*(in++) != ':') is.setstate(std::ios::failbit);
  if (readKey(data, in)) is.setstate(std::ios::failbit);
  if (*(in++) != ':') is.setstate(std::ios::failbit);
  if (*(in++) != ')') is.setstate(std::ios::failbit);

  is >> std::skipws;
  return is;
}
