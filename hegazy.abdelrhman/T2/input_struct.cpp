
#include "input_struct.hpp"
#include "format_guard.hpp"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <stdexcept>

namespace martynov {

    std::string to_lower(std::string str) {
        std::transform(str.begin(), str.end(), str.begin(),
            [](unsigned char c) { return std::tolower(c); });
        return str;
    }

    std::string to_binary(unsigned long long val) {
        if (val == 0) {
            return "0";
        }
        std::string bin;
        while (val > 0) {
            bin = std::string(1, '0' + (val % 2)) + bin;
            val /= 2;
        }
        return bin;
    }

    std::istream& operator>>(std::istream& is, DataStruct& ds) {
        FormatGuard guard(is);

        char ch;
        if (!(is >> ch) || ch != '(') {
            is.setstate(std::ios::failbit);
            return is;
        }

        std::string line;
        std::getline(is, line, ')');
        if (is.fail()) {
            return is;
        }

        std::istringstream iss(line);
        std::string part;
        bool has_key1 = false, has_key2 = false, has_key3 = false;

        while (std::getline(iss, part, ':')) {
            if (part.empty()) {
                continue;
            }

            std::istringstream part_ss(part);
            std::string key;
            part_ss >> key;

            std::string value_line;
            std::getline(part_ss, value_line);
            size_t start = value_line.find_first_not_of(" \t");
            if (start == std::string::npos) {
                is.setstate(std::ios::failbit);
                return is;
            }
            std::string value = value_line.substr(start);

            if (key == "key1") {
                std::string lower = to_lower(value);
                size_t suffix_pos = lower.rfind("ull");
                if (suffix_pos == std::string::npos || suffix_pos != lower.size() - 3) {
                    is.setstate(std::ios::failbit);
                    return is;
                }
                std::string num_str = lower.substr(0, suffix_pos);
                try {
                    ds.key1 = std::stoull(num_str);
                    has_key1 = true;
                }
                catch (...) {
                    is.setstate(std::ios::failbit);
                    return is;
                }
            }
            else if (key == "key2") {
                std::string lower = to_lower(value);
                if (lower.find("0b") != 0) {
                    is.setstate(std::ios::failbit);
                    return is;
                }
                std::string num_str = lower.substr(2);
                try {
                    ds.key2 = std::stoull(num_str, nullptr, 2);
                    has_key2 = true;
                }
                catch (...) {
                    is.setstate(std::ios::failbit);
                    return is;
                }
            }
            else if (key == "key3") {
                size_t start_quote = value.find('"');
                if (start_quote == std::string::npos) {
                    is.setstate(std::ios::failbit);
                    return is;
                }
                size_t end_quote = value.rfind('"');
                if (end_quote == start_quote) {
                    is.setstate(std::ios::failbit);
                    return is;
                }
                ds.key3 = value.substr(start_quote + 1, end_quote - start_quote - 1);
                has_key3 = true;
            }
            else {
                is.setstate(std::ios::failbit);
                return is;
            }
        }

        if (!has_key1 || !has_key2 || !has_key3) {
            is.setstate(std::ios::failbit);
        }

        return is;
    }

    std::ostream& operator<<(std::ostream& os, const DataStruct& ds) {
        FormatGuard guard(os);
        os << "(:key1 " << ds.key1 << "ull:key2 0b" << to_binary(ds.key2) << ":key3 \"" << ds.key3 << "\":)";
        return os;
    }

}  