#include "DataStruct.h"
#include "ParserUtils.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

std::istream& operator>>(std::istream& in, DataStruct& data) {
  std::string line;
  if (!std::getline(in, line)) {
    return in;
  }

  if (line.empty() || line.front() != '(' || line.back() != ')') {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::string content = line.substr(1, line.size() - 2);
  std::istringstream iss(content);
  bool hasKey1 = false, hasKey2 = false, hasKey3 = false;
  std::string token;

  try {
    while (std::getline(iss, token, ':')) {
      if (token.empty())
        continue;
      std::istringstream tokenStream(token);
      std::string fieldName;
      tokenStream >> fieldName;

      if (fieldName == "key1") {
        std::string value;
        tokenStream >> value;
        if (ParserUtils::parseHexULL(value, data.key1)) {
          hasKey1 = true;
        } else {
          in.setstate(std::ios::failbit);
          return in;
        }
      }
      else if (fieldName == "key2") {
        std::string value;
        std::getline(tokenStream >> std::ws, value);
        if (ParserUtils::parseComplex(value, data.key2)) {
          hasKey2 = true;
        } else {
            in.setstate(std::ios::failbit);
            return in;
            }
        }
        else if (fieldName == "key3") {
          std::string value;
          std::getline(tokenStream >> std::ws, value);
          if (ParserUtils::parseString(value, data.key3)) {
            hasKey3 = true;
          } else {
              in.setstate(std::ios::failbit);
              return in;
            }
          }
      }

      if (!hasKey1 || !hasKey2 || !hasKey3) {
        in.setstate(std::ios::failbit);
      }
  }
  catch (...) {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data) {
  out << "(:key1 0x" << std::hex << std::uppercase << data.key1
<< ":key2 #c(" << data.key2.real() << " " << data.key2.imag()
<< "):key3 \"" << data.key3 << "\":)";
  return out;
}

bool compareDataStruct(const DataStruct& a, const DataStruct& b) {
  if (a.key1 != b.key1) {
    return a.key1 < b.key1;
  }
  double modA = std::abs(a.key2);
  double modB = std::abs(b.key2);
  if (std::abs(modA - modB) > 1e-10) {
    return modA < modB;
  }
  return a.key3.length() < b.key3.length();
}
