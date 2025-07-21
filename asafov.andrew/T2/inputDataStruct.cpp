#include "datastruct.h"
#include <iomanip>
#include <string>

std::istream& asafov::operator>>(std::istream& in, asafov::DataStruct& data)
{
  std::istream::sentry centry(in);
  if (!centry) return in;

  char ch = 0;
  in >> std::ws >> ch;
  if (!in || ch != '(') {
    in.setstate(std::ios::failbit);
    std::string discard;
    std::getline(in, discard);
    return in;
  }

  bool hasKey1 = false, hasKey2 = false, hasKey3 = false;

  while (true)
  {
    in >> std::ws >> ch;
    if (!in || ch != ':') {
      in.setstate(std::ios::failbit);
      std::string discard;
      std::getline(in, discard);
      return in;
    }

    std::string key;
    in >> key;
    if (!in) {
      in.setstate(std::ios::failbit);
      std::string discard;
      std::getline(in, discard);
      return in;
    }

    if (key == "key1")
    {
      std::string literal;
      in >> literal;
      if (!in || (literal.substr(0,2) != "0b" && literal.substr(0,2) != "0B")) {
        in.setstate(std::ios::failbit);
        std::string discard;
        std::getline(in, discard);
        return in;
      }

      try {
        data.key1 = std::stoull(literal.substr(2), nullptr, 2);
        hasKey1 = true;
      } catch (...) {
        in.setstate(std::ios::failbit);
        std::string discard;
        std::getline(in, discard);
        return in;
      }
    }
    else if (key == "key2")
    {
      char hash = 0, c = 0, open = 0;
      in >> std::ws >> hash >> c >> open;
      if (!in || hash != '#' || c != 'c' || open != '(') {
        in.setstate(std::ios::failbit);
        std::string discard;
        std::getline(in, discard);
        return in;
      }

      double re = 0.0, im = 0.0;
      char close = 0;
      in >> re >> im >> close;
      if (!in || close != ')') {
        in.setstate(std::ios::failbit);
        std::string discard;
        std::getline(in, discard);
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
        std::string discard;
        std::getline(in, discard);
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
        std::string discard;
        std::getline(in, discard);
        return in;
      }
    }

    in >> std::ws >> ch;
    if (!in) {
      in.setstate(std::ios::failbit);
      std::string discard;
      std::getline(in, discard);
      return in;
    }

    if (ch == ')') break;
    else if (ch != ':') {
      in.setstate(std::ios::failbit);
      std::string discard;
      std::getline(in, discard);
      return in;
    }
  }

  if (!(hasKey1 && hasKey2 && hasKey3)) {
    in.setstate(std::ios::failbit);
    std::string discard;
    std::getline(in, discard);
  }

  return in;
}
