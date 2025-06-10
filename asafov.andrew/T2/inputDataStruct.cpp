#include "datastruct.h"
#include <sstream>
#include <cctype>
#include <algorithm>
#include <vector>
#include <utility>

namespace
{
    unsigned long long parseULLBin(const std::string& str)
    {
        if (str.length() < 3 || (str[0] != '0' || (str[1] != 'b' && str[1] != 'B')))
            return 0;

        unsigned long long result = 0;
        for (size_t i = 2; i < str.size(); ++i)
        {
            result <<= 1;
            if (str[i] == '1')
                result |= 1;
            else if (str[i] != '0')
                return 0;
        }
        return result;
    }

    std::complex<double> parseCmpLsp(const std::string& str)
    {
        if (str.length() < 5 || str[0] != '#' || str[1] != 'c' || str[2] != '(')
            return {0.0, 0.0};

        std::string content = str.substr(3, str.size() - 4);
        std::istringstream iss(content);
        double real, imag;
        iss >> real >> imag;
        return {real, imag};
    }

    bool extractField(const std::string& line, const std::string& fieldName, 
                     size_t& pos, std::string& value)
    {
        size_t field_pos = line.find(fieldName, pos);
        if (field_pos == std::string::npos)
            return false;

        size_t value_start = field_pos + fieldName.length();
        size_t value_end;

        if (fieldName == ":key3 \"") {
            value_end = line.find('"', value_start);
            if (value_end == std::string::npos)
                return false;
            value = line.substr(value_start, value_end - value_start);
            pos = value_end + 2; // skip quote and :
        } else {
            value_end = line.find(':', value_start);
            if (value_end == std::string::npos)
                return false;
            value = line.substr(value_start, value_end - value_start);
            pos = value_end;
        }

        return true;
    }
}

std::istream& asafov::operator>>(std::istream& is, DataStruct& data)
{
    std::string line;
    if (!std::getline(is, line)) {
        return is;
    }

    DataStruct temp;
    bool has_key1 = false, has_key2 = false, has_key3 = false;
    size_t pos = 0;

    try {
        while (pos < line.length()) {
            if (line.find(":key1 ", pos) == pos) {
                std::string value;
                if (!extractField(line, ":key1 ", pos, value)) break;
                temp.key1 = parseULLBin(value);
                if (temp.key1 == 0 && value != "0b0") break;
                has_key1 = true;
            }
            else if (line.find(":key2 ", pos) == pos) {
                std::string value;
                if (!extractField(line, ":key2 ", pos, value)) break;
                temp.key2 = parseCmpLsp(value);
                if (temp.key2 == std::complex<double>{0.0, 0.0} && 
                    value != "#c(0.0 0.0)" && 
                    value != "#c(0 0)" && 
                    value != "#c(0. 0.)") break;
                has_key2 = true;
            }
            else if (line.find(":key3 \"", pos) == pos) {
                std::string value;
                if (!extractField(line, ":key3 \"", pos, value)) break;
                temp.key3 = value;
                has_key3 = true;
            }
            else {
                pos++;
            }
        }

        // Check if all fields were found and the line ends properly
        if (!has_key1 || !has_key2 || !has_key3 || 
            line.substr(pos) != ":" && line.substr(pos) != "") {
            is.setstate(std::ios::failbit);
            return is;
        }
    } catch (...) {
        is.setstate(std::ios::failbit);
        return is;
    }

    data = temp;
    return is;
}