#ifndef KEYI_HPP
#define KEYI_HPP

#include <istream>
#include <string>
#include <utility>

namespace smirnov
{
    struct BinKey
    {
        unsigned long long& value;
    };

    std::istream& operator>>(std::istream& in, BinKey&& key);

    struct DblLit
    {
        double& key1;
    };

    std::istream& operator>>(std::istream& in, DblLit&& data);

    struct STRKey
    {
        std::string& str;
    };

    std::istream& operator>>(std::istream& in, STRKey&& key);
}

#endif
