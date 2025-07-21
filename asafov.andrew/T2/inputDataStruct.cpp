#include "datastruct.h"
#include <istream>
#include <complex>
#include <cctype>
#include <string>
#include <cstdlib>

namespace {
  using It = std::istreambuf_iterator<char>;

  void skipWhitespace(It& it, const It& end) {
    while (it != end && std::isspace(static_cast<unsigned char>(*it))) {
      ++it;
    }
  }

  bool matchPrefix(It& it, const It& end, const std::string& prefix) {
    auto tmp = it;
    for (char ch : prefix) {
      if (tmp == end || *tmp != ch) return false;
      ++tmp;
    }
    it = tmp;
    return true;
  }

  std::string parseIdentifier(It& it, const It& end) {
    std::string result;
    while (it != end && std::isalpha(static_cast<unsigned char>(*it))) {
      result += *it;
      ++it;
    }
    return result;
  }

  unsigned long long parseBinary(It& it, const It& end, std::istream& is) {
    if (!matchPrefix(it, end, "0b")) {
      is.setstate(std::ios::failbit);
      return 0;
    }
    std::string bits;
    while (it != end && (*it == '0' || *it == '1')) {
      bits += *it;
      ++it;
    }
    if (bits.empty()) {
      is.setstate(std::ios::failbit);
      return 0;
    }
    unsigned long long value = 0;
    for (char b : bits) {
      value = (value << 1) | (b - '0');
    }
    return value;
  }

  std::complex<double> parseComplex(It& it, const It& end, std::istream& is) {
    if (!matchPrefix(it, end, "#c(")) {
      is.setstate(std::ios::failbit);
      return {};
    }
    skipWhitespace(it, end);
    std::string rePart, imPart;
    while (it != end && (std::isdigit(*it) || *it == '.' || *it == '-' || *it == '+')) {
      rePart += *it;
      ++it;
    }
    skipWhitespace(it, end);
    while (it != end && *it != ')') {
      imPart += *it;
      ++it;
    }
    if (it == end || *it != ')') {
      is.setstate(std::ios::failbit);
      return {};
    }
    ++it;
    char* ptr = nullptr;
    double re = std::strtod(rePart.c_str(), &ptr);
    if (ptr == rePart.c_str()) {
      is.setstate(std::ios::failbit);
      return {};
    }
    double im = 0.0;
    if (!imPart.empty()) {
      im = std::strtod(imPart.c_str(), &ptr);
      if (ptr == imPart.c_str()) {
        is.setstate(std::ios::failbit);
        return {};
      }
    }
    return {re, im};
  }

  std::string parseQuotedString(It& it, const It& end, std::istream& is) {
    if (it == end || *it != '"') {
      is.setstate(std::ios::failbit);
      return {};
    }
    ++it;
    std::string str;
    while (it != end && *it != '"') {
      str += *it;
      ++it;
    }
    if (it == end || *it != '"') {
      is.setstate(std::ios::failbit);
      return {};
    }
    ++it;
    return str;
  }

  void skipToNextLine(It& it, const It& end) {
    while (it != end) {
      if (*it == '\n') {
        ++it;
        break;
      }
      ++it;
    }
  }
}

std::istream& asafov::operator>>(std::istream& is, DataStruct& data) {
  std::istream::sentry guard(is);
  if (!guard) {
    is.setstate(std::ios::failbit);
    return is;
  }

  std::istreambuf_iterator<char> it(is), end;

  while (it != end) {
    DataStruct temp;
    bool has_key1 = false, has_key2 = false, has_key3 = false;

    skipWhitespace(it, end);
    while (it != end && *it == ':') {
      ++it;
      skipWhitespace(it, end);
      std::string key = parseIdentifier(it, end);
      skipWhitespace(it, end);

      if (key == "key1") {
        temp.key1 = parseBinary(it, end, is);
        has_key1 = !is.fail();
      } else if (key == "key2") {
        temp.key2 = parseComplex(it, end, is);
        has_key2 = !is.fail();
      } else if (key == "key3") {
        temp.key3 = parseQuotedString(it, end, is);
        has_key3 = !is.fail();
      } else {
        is.setstate(std::ios::failbit);
      }

      if (is.fail()) {
        skipToNextLine(it, end);
        is.clear();
        if (is.eof()) break;
        it = std::istreambuf_iterator<char>(is);
        continue;
      }

      skipWhitespace(it, end);
    }

    if (has_key1 && has_key2 && has_key3) {
      data = temp;
      return is;
    }

    skipToNextLine(it, end);
    is.clear();
    if (is.eof()) break;
    it = std::istreambuf_iterator<char>(is);
  }

  is.setstate(std::ios::failbit);
  return is;
}
