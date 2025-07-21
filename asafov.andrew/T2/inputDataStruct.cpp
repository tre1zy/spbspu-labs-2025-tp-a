#include "datastruct.h"
#include <iomanip>
#include <string>

std::istream& asafov::operator>>(std::istream& in, asafov::DataStruct& data)
{
  char ch = 0;
  in >> std::ws >> ch;
  if (!in || ch != '(') {
    in.setstate(std::ios::failbit);
    return in;
  }

  bool hasKey1 = false, hasKey2 = false, hasKey3 = false;

  while (true)
  {
    in >> std::ws >> ch;
    if (!in || ch != ':') {
      in.setstate(std::ios::failbit);
      return in;
    }

    std::string key;
    in >> key;
    if (!in) {
      in.setstate(std::ios::failbit);
      return in;
    }

    if (key == "key1")
    {
      std::string literal;
      in >> literal;
      if (!in) {
        in.setstate(std::ios::failbit);
        return in;
      }
      if (literal.substr(0, 2) == "0b" || literal.substr(0, 2) == "0B") {
        try {
          data.key1 = std::stoull(literal.substr(2), nullptr, 2);
          hasKey1 = true;
        } catch (...) {
          in.setstate(std::ios::failbit);
          return in;
        }
      } else {
        in.setstate(std::ios::failbit);
        return in;
      }
    }
    else if (key == "key2")
    {
      char hash = 0, c = 0, open = 0;
      in >> std::ws >> hash >> c >> open;
      if (!in || hash != '#' || c != 'c' || open != '(') {
        in.setstate(std::ios::failbit);
        return in;
      }

      double re = 0.0, im = 0.0;
      char close = 0;
      in >> re >> im >> close;
      if (!in || close != ')') {
        in.setstate(std::ios::failbit);
        return in;
      }

      data.key2 = std::complex<double>{re, im};
      hasKey2 = true;
    }
    else if (key == "key3")
    {
      std::string quoted;
      in >> std::quoted(quoted);
      if (!in) {
        in.setstate(std::ios::failbit);
        return in;
      }

      data.key3 = quoted;
      hasKey3 = true;
    }
    else
    {
      std::string dummy;
      in >> dummy;
      if (!in) {
        in.setstate(std::ios::failbit);
        return in;
      }
    }

    in >> std::ws >> ch;
    if (!in) {
      in.setstate(std::ios::failbit);
      return in;
    }

    if (ch == ')') break;
    if (ch != ':') {
      in.setstate(std::ios::failbit);
      return in;
    }
  }

  if (!(hasKey1 && hasKey2 && hasKey3)) {
    in.setstate(std::ios::failbit);
  }

  return in;
}
