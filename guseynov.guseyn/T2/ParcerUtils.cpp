#include "ParserUtils.h"
#include <sstream>
#include <iomanip>
#include <cctype>
#include <algorithm>

namespace ParserUtils {

  bool parseHexULL(const std::string& str, unsigned long long& result) {
    if (str.size() < 3 || (str.substr(0, 2) != "0x" && str.substr(0, 2) != "0X")) {
      return false;
    }
    std::string hexStr = str.substr(2);
    hexStr.erase(std::remove_if(hexStr.begin(), hexStr.end(), [](char c) { return !std::isxdigit(c); }), hexStr.end());
    if (hexStr.empty())
      return false;
    std::istringstream iss(hexStr);
    iss >> std::hex >> result;
    return !iss.fail() && iss.eof();
  }

  bool parseComplex(const std::string& str, std::complex<double>& result) {
    if (str.size() < 5 || str.substr(0, 3) != "#c(" || str.back() != ')') {
      return false;
    }
    std::string content = str.substr(3, str.size() - 4);
    std::replace(content.begin(), content.end(), '-', ' ');
    std::istringstream iss(content);
    double real, imag;
    if (iss >> real >> imag) {
      result = std::complex<double>(real, imag);
      return true;
    }
    return false;
  }

  bool parseString(const std::string& str, std::string& result) {
    if (str.size() < 2 || str.front() != '"' || str.back() != '"') {
        return false;
    }
    result = str.substr(1, str.size() - 2);
    return true;
  }

}
