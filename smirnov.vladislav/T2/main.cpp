#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include "data.hpp"
#include <sstream>

int main()
{
    std::vector< smirnov::DataStruct > data;
    std::string line;

    while (true)
    {
        std::copy
        (
            std::getline(std::cin, line);
        if (!std::cin || line.empty())
        {
            break;
        }

        std::istringstream iss(line);
        smirnov::DataStruct temp;
        if (iss >> temp)
        {
            data.push_back(temp);
        }
    }

    std::sort(data.begin(), data.end(), smirnov::comparator);

    std::copy(
        std::begin(data),
        std::end(data),
        std::ostream_iterator< smirnov::DataStruct >(std::cout, "\n")
    );

    return 0;
}
