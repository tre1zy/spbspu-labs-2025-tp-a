#ifndef PARCERUTILS_H
#define PARCERUTILS_H

#include <string>
#include <complex>
#include <utility>

namespace ParserUtils {
bool parseComplex(const std::string& str, std::complex<double>& result);
bool parseRational(const std::string& str, std::pair<long long, unsigned long long>& result);
bool parseString(const std::string& str, std::string& result);
}

#endif
