
#include "data_struct.hpp"
#include "input_struct.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>

int main() {
    using namespace martynov;
    std::vector<DataStruct> data;

    DataStruct ds;
    while (std::cin) {
        if (std::cin >> ds) {
            data.push_back(ds);
        }
        else {
            if (std::cin.eof()) {
                break;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::sort(data.begin(), data.end(), compare);

    std::copy(data.begin(), data.end(), std::ostream_iterator<DataStruct>(std::cout, "\n"));

    return 0;
}