#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "DataStruct.hpp"

int main() {
    std::vector<DataStruct> data;
    using istream_it = std::istream_iterator<DataStruct>;
    using ostream_it = std::ostream_iterator<DataStruct>;
    
    std::copy(istream_it{ std::cin },
        istream_it{},
        std::back_inserter(data));

    std::sort(data.begin(), data.end(), compareDataStruct);
    std::copy(data.begin(), data.end(),
        ostream_it{ std::cout, "\n" });

    return 0;
}