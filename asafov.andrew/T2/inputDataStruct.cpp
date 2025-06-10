#include "datastruct.h"
#include <sstream>
#include <cctype>
#include <algorithm>

namespace
{
    unsigned long long parseULLBin(const std::string& str)
    {
        if (str.length() < 3 || (str[0] != '0' && str[1] != 'b' && str[0] != '0' && str[1] != 'B'))
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
    std::getline(is, line);

    size_t key1_pos = line.find(":key1 ");
    if (key1_pos == std::string::npos) return is;
    size_t key1_end = line.find(':', key1_pos + 1);
    if (key1_end == std::string::npos) return is;
    std::string key1_str = line.substr(key1_pos + 6, key1_end - (key1_pos + 6));
    data.key1 = parseULLBin(key1_str);

    size_t key2_pos = line.find(":key2 ");
    if (key2_pos == std::string::npos) return is;
    size_t key2_end = line.find(':', key2_pos + 1);
    if (key2_end == std::string::npos) return is;
    std::string key2_str = line.substr(key2_pos + 6, key2_end - (key2_pos + 6));
    data.key2 = parseCmpLsp(key2_str);

    size_t key3_pos = line.find(":key3 \"");
    if (key3_pos == std::string::npos) return is;
    size_t key3_end = line.find("\"", key3_pos + 7);
    if (key3_end == std::string::npos) return is;
    data.key3 = line.substr(key3_pos + 7, key3_end - (key3_pos + 7));

    return is;
}
