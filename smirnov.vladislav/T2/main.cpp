#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include "data.hpp"
#include <sstream>
#include <limits>

int main()
{
    std::vector< smirnov::DataStruct > data;
    while (std::cin)
    {
        std::copy
        (
            std::istream_iterator< smirnov::DataStruct >(std::cin),
            std::istream_iterator< smirnov::DataStruct >(),
            std::back_inserter(data)
        );
        if (!std::cin.eof())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        }
    }

    std::sort(data.begin(), data.end(), [](const smirnov::DataStruct& a, const smirnov::DataStruct& b)
        {
            if (std::abs(a.key1 - b.key1) > 1e-9)
            {
                return a.key1 < b.key1;
            }

            if (a.key2 != b.key2)
            {
                return a.key2 < b.key2;
            }

            return a.key3.size() < b.key3.size();
        });

    std::copy(
        std::begin(data),
        std::end(data),
        std::ostream_iterator< smirnov::DataStruct >(std::cout, "\n")
    );

    return 0;
}
