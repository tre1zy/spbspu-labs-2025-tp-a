#include "dataStruct.hpp"
#include "scopeGuard.hpp"
#include <bitset>
#include <cctype>
#include <iostream>
#include <set>
#include <string>

std::istream &kostyukov::operator>>(std::istream &in, DelimiterIO &&dest) {
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (std::tolower(c) != std::tolower(dest.expected))) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream &kostyukov::operator>>(std::istream &in, StringIO &&dest) {
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  std::string temp;
  in >> DelimiterIO{'"'};
  std::getline(in, temp, '"');
  in >> DelimiterIO{':'};
  if (in) {
    dest.ref = temp;
  }
  return in;
}

std::istream &kostyukov::operator>>(std::istream &in, UllIO &&dest) {
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  in >> DelimiterIO{'0'};
  char prefix = '0';
  in >> prefix;
  int base = 0;
  prefix = std::tolower(prefix);
  if (prefix == 'b') {
    base = 2;
  } else if (prefix == 'x') {
    base = 16;
  } else {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::string digits;
  std::getline(in, digits, ':');
  if (digits.empty()) {
    in.setstate(std::ios::failbit);
    return in;
  }
  try {
    size_t processedChars = 0;
    unsigned long long value = std::stoull(digits, &processedChars, base);
    if (processedChars != digits.length()) {
      in.setstate(std::ios::failbit);
    } else {
      dest.ref = value;
    }
  } catch (const std::exception &) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream &kostyukov::operator>>(std::istream &in, DataStruct &dest) {
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  in >> DelimiterIO{'('} >> DelimiterIO{':'};
  DataStruct temp{};
  std::set<size_t> processedKeys;
  const size_t COUNT_KEYS_EXPECTED = 3;
  for (size_t i = 0; i < COUNT_KEYS_EXPECTED; i++) {
    std::string key = "";
    if ((in >> key) && (key.size() == 4) && (key.substr(0, 3) == "key")) {
      char keyNum = key.back();
      switch (keyNum) {
      case '1':
        in >> UllIO{temp.key1, UllIO::Format::BIN};
        break;
      case '2':
        in >> UllIO{temp.key2, UllIO::Format::HEX};
        break;
      case '3':
        in >> StringIO{temp.key3};
        break;
      default:
        in.setstate(std::ios::failbit);
      }
    } else {
      in.setstate(std::ios::failbit);
    }
  }
  in >> DelimiterIO{')'};
  if (in) {
    dest = temp;
  }
  return in;
}

std::ostream &kostyukov::operator<<(std::ostream &out, const UllIO &&dest) {
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  kostyukov::ScopeGuard scopeGrd(out);
  if (dest.format == UllIO::Format::BIN) {
    std::string binStr = std::bitset<64>(dest.ref).to_string();
    size_t first_one = binStr.find('1');
    out << "0b";
    if (first_one != std::string::npos) {
      out << binStr.substr(first_one);
    } else {
      out << '0';
    }
  } else {
    out << "0x" << std::hex << std::uppercase << dest.ref;
  }
  return out;
}

std::ostream &kostyukov::operator<<(std::ostream &out, const StringIO &&dest) {
  return out << '"' << dest.ref << '"';
}

std::ostream &kostyukov::operator<<(std::ostream &out, const DataStruct &dest) {
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  unsigned long long ullBin = dest.key1;
  unsigned long long ullHex = dest.key2;
  std::string string = dest.key3;
  out << "(:key1 " << UllIO{ullBin, UllIO::BIN} << ":key2 "
      << UllIO{ullHex, UllIO::HEX} << ":key3 " << StringIO{string} << ":)";
  return out;
}

bool kostyukov::comparator(const DataStruct &lhs, const DataStruct &rhs) {
  if (lhs.key1 != rhs.key1) {
    return lhs.key1 < rhs.key1;
  } else if (lhs.key2 != rhs.key2) {
    return lhs.key2 < rhs.key2;
  }
  return lhs.key3.size() < rhs.key3.size();
}
