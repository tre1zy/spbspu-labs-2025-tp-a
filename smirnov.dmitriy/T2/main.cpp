#include "data_struct.hpp"
#include <algorithm>
#include <iterator>
#include <vector>

int main() {
    using namespace smirnov;

    std::vector<DataStruct> data;

    std::copy(std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(data));

    std::sort(data.begin(), data.end(), compareDataStruct);

    std::copy(data.cbegin(), data.cend(),
        std::ostream_iterator<DataStruct>(std::cout, "\n"));

    return 0;
}

