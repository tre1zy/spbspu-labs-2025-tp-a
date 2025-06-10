#include "datastruct.h"
#include <sstream>
#include <cctype>
#include <algorithm>

namespace
{
    unsigned long long parseULLBin(const std::string& str)
    {
        if (str.length() < 3 || (str[0] != '0' && str[1] != 'b' && str[0] != '0' &&str[1] != 'B'))
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
}

std::istream& asafov::operator>>(std::istream& is, DataStruct& data)
{
    std::string line;
    if (!std::getline(is, line)) {
        return is;
    }

    DataStruct temp;
    bool valid = true;

    size_t key1_pos = line.find(":key1 ");
    if (key1_pos == std::string::npos) valid = false;
    size_t key1_end = line.find(':', key1_pos + 1);
    if (key1_end == std::string::npos) valid = false;
    if (valid) {
        std::string key1_str = line.substr(key1_pos + 6, key1_end - (key1_pos + 6));
        temp.key1 = parseULLBin(key1_str);
        if (temp.key1 == 0 && key1_str != "0b0") valid = false;
    }

    size_t key2_pos = line.find(":key2 ");
    if (key2_pos == std::string::npos || key2_pos < key1_end) valid = false;
    size_t key2_end = line.find(':', key2_pos + 1);
    if (key2_end == std::string::npos) valid = false;
    if (valid) {
        std::string key2_str = line.substr(key2_pos + 6, key2_end - (key2_pos + 6));
        temp.key2 = parseCmpLsp(key2_str);
        if (temp.key2 == std::complex<double>{0.0, 0.0} && 
            key2_str != "#c(0.0 0.0)" && 
            key2_str != "#c(0 0)" && 
            key2_str != "#c(0. 0.)") {
            valid = false;
        }
    }

    size_t key3_pos = line.find(":key3 \"");
    if (key3_pos == std::string::npos || key3_pos < key2_end) valid = false;
    size_t closing_quote = line.find('"', key3_pos + 7);
    if (closing_quote == std::string::npos) valid = false;
    size_t final_colon = line.find(':', closing_quote + 1);
    if (final_colon == std::string::npos) valid = false;
    
    if (valid) {
        temp.key3 = line.substr(key3_pos + 7, closing_quote - (key3_pos + 7));
    }

    if (valid) {
        data = temp;
    } else {
        is.setstate(std::ios::failbit);
    }

    return is;
}