#include "ParcerUtils.h"
#include <sstream>
#include <algorithm>

namespace ParserUtils {

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

bool parseRational(const std::string& str, std::pair<long long, unsigned long long>& result) {
  if (str.size() < 7 || str.substr(0, 2) != "(:") {
    return false;
  }
  size_t nPos = str.find(":N");
  size_t dPos = str.find(":D");
  if (nPos == std::string::npos || dPos == std::string::npos) {
    return false;
  }
  try {
    std::string nStr = str.substr(nPos + 2, dPos - (nPos + 2));
    std::istringstream nIss(nStr);
    nIss >> result.first;
    std::string dStr = str.substr(dPos + 2);
    dStr = dStr.substr(0, dStr.find(":"));
    std::istringstream dIss(dStr);
    dIss >> result.second;
    return !nIss.fail() && !dIss.fail() && result.second != 0;
  }
  catch (...) {
    return false;
  }
}

bool parseString(const std::string& str, std::string& result) {
  if (str.size() < 2 || str.front() != '"' || str.back() != '"') {
    return false;
  }
  result = str.substr(1, str.size() - 2);
  return true;
}

}
