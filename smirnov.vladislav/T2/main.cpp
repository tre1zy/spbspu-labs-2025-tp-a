#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include "data.hpp"
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

    std::sort(data.begin(), data.end());

    std::copy(
        std::begin(data),
        std::end(data),
        std::ostream_iterator< smirnov::DataStruct >(std::cout, "\n")
    );

    return 0;
}
