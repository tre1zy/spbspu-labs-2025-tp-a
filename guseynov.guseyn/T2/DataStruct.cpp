#include "DataStruct.h"
#include "ParcerUtils.h"
#include <sstream>
#include <iomanip>
#include <cmath>

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
      if (token.empty()) continue;
      std::istringstream tokenStream(token);
      std::string fieldName;
      tokenStream >> fieldName;
      if (fieldName == "key1") {
        std::string value;
        std::getline(tokenStream >> std::ws, value);
        if (ParserUtils::parseComplex(value, data.key1)) {
          hasKey1 = true;
        } else {
          in.setstate(std::ios::failbit);
          return in;
        }
      }
      else if (fieldName == "key2") {
        std::string value;
        std::getline(tokenStream >> std::ws, value);
        if (ParserUtils::parseRational(value, data.key2)) {
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
  out << "(:key1 #c(" << std::fixed << std::setprecision(1)
    << data.key1.real() << " " << data.key1.imag()
    << "):key2 (:N " << data.key2.first << ":D " << data.key2.second
    << ":):key3 \"" << data.key3 << "\":)";
  return out;
}

bool compareDataStruct(const DataStruct& a, const DataStruct& b) {
  double modA = std::abs(a.key1);
  double modB = std::abs(b.key1);
  if (std::abs(modA - modB) > 1e-10) {
    return modA < modB;
  }
  double ratioA = static_cast<double>(a.key2.first) / a.key2.second;
  double ratioB = static_cast<double>(b.key2.first) / b.key2.second;
  if (std::abs(ratioA - ratioB) > 1e-10) {
    return ratioA < ratioB;
  }
  return a.key3.length() < b.key3.length();
}
