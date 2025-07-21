#include "datastruct.h"
#include <iomanip>
#include <string>

namespace asafov
{
  std::istream& operator>>(std::istream& in, DataStruct& data)
  {
    char ch;
    in >> std::ws >> ch;
    if (ch != '(') {
      in.setstate(std::ios::failbit);
      return in;
    }

    bool hasKey1 = false, hasKey2 = false, hasKey3 = false;

    while (in >> std::ws >> ch && ch == ':') {
      std::string key;
      in >> key;

      if (key == "key1") {
        std::string val;
        in >> val;
        if (val.size() >= 3 && (val.substr(0, 2) == "0b" || val.substr(0, 2) == "0B")) {
          try {
            data.key1 = std::stoull(val.substr(2), nullptr, 2);
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
      else if (key == "key2") {
        char hash, c, open, close;
        double re, im;
        in >> std::ws >> hash >> c >> open;
        if (hash == '#' && c == 'c' && open == '(') {
          in >> re >> im >> close;
          if (close == ')') {
            data.key2 = std::complex<double>(re, im);
            hasKey2 = true;
          } else {
            in.setstate(std::ios::failbit);
            return in;
          }
        } else {
          in.setstate(std::ios::failbit);
          return in;
        }
      }
      else if (key == "key3") {
        in >> std::quoted(data.key3);
        hasKey3 = true;
      }
      else {
        std::string dummy;
        in >> dummy;
      }

      in >> std::ws >> ch;
      if (ch != ':') break;
    }

    if (ch != ')') {
      in.setstate(std::ios::failbit);
      return in;
    }

    if (!(hasKey1 && hasKey2 && hasKey3)) {
      in.setstate(std::ios::failbit);
    }

    return in;
  }
}
