#include "DataStruct.hpp"
#include <iomanip>
#include "StreamGuard.hpp"

std::istream& holodilov::operator>>(std::istream& in, DataStruct& dataStruct)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  DataStruct dataStructTemp;
  in >> io::DelimIO('(');
  for (int i = 0; i < 3; ++i)
  {
    in >> io::DelimIO(':');
    io::KeyNameIO keyName(0);
    in >> keyName;
    if (keyName.keyNumber == 1)
    {
      in >> io::UllOctIO(dataStructTemp.key1);
    }
    else if (keyName.keyNumber == 2)
    {
      in >> io::ComplexIO(dataStructTemp.key2);
    }
    else if (keyName.keyNumber == 3)
    {
      in >> io::StringIO(dataStructTemp.key3);
    }
    if (!in)
    {
      return in;
    }
  }
  in >> io::DelimIO(':');
  in >> io::DelimIO(')');
  if (in)
  {
    dataStruct = dataStructTemp;
  }
  return in;
}

std::ostream& holodilov::operator<<(std::ostream& out, const DataStruct& dataStruct)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  StreamGuard streamGuard(out);
  out << "(:key1 0" << dataStruct.key1;
  out << std::fixed << std::setprecision(1);
  out << ":key2 #c(";
  out << dataStruct.key2.real() << " " << dataStruct.key2.imag() << ")";
  out << ":key3 \"" << dataStruct.key3 << "\":)";
  return out;
}

bool holodilov::DataStruct::operator<(const DataStruct& other) const
{
  if (key1 != other.key1)
  {
    return key1 < other.key1;
  }
  if (key2 != other.key2)
  {
    return std::abs(key2) < std::abs(other.key2);
  }
  return key3.size() < other.key3.size();
}

std::istream& holodilov::io::operator>>(std::istream& in, DelimIO&& delim)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char ch;
  in >> ch;
  if (ch != delim.delim)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& holodilov::io::operator>>(std::istream& in, UllOctIO&& ullOct)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimIO('0');
  in >> ullOct.ullOctRef;
  if (in)
  {
    if (!ullOct.checkOctFormat())
    {
      in.setstate(std::ios::failbit);
    }
  }
  return in;
}

bool holodilov::io::UllOctIO::checkOctFormat() const
{
  unsigned long long oct = ullOctRef;
  while (oct != 0)
  {
    if (oct % 10 >= 8)
    {
      return false;
    }
    oct /= 10;
  }
  return true;
}

std::istream& holodilov::io::operator>>(std::istream& in, ComplexIO&& complex)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimIO('#');
  in >> DelimIO('c');
  in >> DelimIO('(');
  double complexRe = 0;
  double complexIm = 0;
  in >> complexRe >> complexIm;
  in >> DelimIO(')');
  if (in)
  {
    complex.complexRef = std::complex< double >(complexRe, complexIm);
  }
  return in;
}

std::istream& holodilov::io::operator>>(std::istream& in, StringIO&& str)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimIO('\"');
  return std::getline(in, str.strRef, '\"');
}

std::istream& holodilov::io::operator>>(std::istream& in, KeyNameIO& keyName)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimIO('k') >> DelimIO('e') >> DelimIO('y');
  if (in)
  {
    in >> keyName.keyNumber;
    if (keyName.keyNumber != 1 && keyName.keyNumber != 2 && keyName.keyNumber != 3)
    {
      in.setstate(std::ios::failbit);
    }
  }
  return in;
}
