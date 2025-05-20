#include <iostream>
#include <vector>
#include <algorithm>
#include "data_struct.hpp"

int main() {
    using smirnov::DataStruct;
    using smirnov::compareDataStruct;

    std::vector<DataStruct> data;
    DataStruct ds;

    while (std::cin >> ds) {
        data.push_back(ds);
    }

    std::sort(data.begin(), data.end(), compareDataStruct);

    for (const auto& elem : data) {
        std::cout << elem << std::endl;
    }

    return 0;
}

