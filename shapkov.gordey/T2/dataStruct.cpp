#include "dataStruct.hpp"
#include <delimiter.hpp>

std::istream& shapkov::operator>>(std::istream& in, DataStruct& rhs)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  shapkov::DataStruct input;
  using check = delimiterIO;
  using label = labelIO;
  using dbl = doubleScientificIO;
  using ratio = ratioIO;
  using str = stringIO;
  //bool eachKey[3]{};
  in >> check{ '(' } >> check{ ':' } >> label{ "key1" };
  in >> dbl{ input.key1 };
  in >> check{ ':' } >> label{ "key2" };
  in >> ratio{ input.key2 };
  in >> check{ ':' } >> label{ "key3" };
  in >> str{ input.key3 };
  in >> check{ ':' } >> check{ ')' };
  if (in)
  {
    rhs = input;
  }
  return in;
}

std::ostream& shapkov::operator<<(std::ostream& out, const DataStruct& rhs)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  out << "(:key1 ";
  out << rhs.key1;
  out << ":key2 (:N ";
  out << rhs.key2.first;
  out << ":D ";
  out << rhs.key2.second;
  out << ":):key3 \"";
  out << rhs.key3 << "\":)";
  return out;
}
