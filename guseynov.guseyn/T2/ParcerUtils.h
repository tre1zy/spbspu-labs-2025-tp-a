#ifndef PARSERUTILS_H
#define PARSERUTILS_H

#include <string>
#include <complex>

namespace ParserUtils {
bool parseHexULL(const std::string& str, unsigned long long& result);
bool parseComplex(const std::string& str, std::complex<double>& result);
bool parseString(const std::string& str, std::string& result);
}

#endif
