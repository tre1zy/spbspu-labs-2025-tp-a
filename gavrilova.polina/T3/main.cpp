#include <iostream>
#include "Polygon/Polygon.hpp"
#include "Utils/Utils.hpp"

int main(int argc, char* argv[]) {
    std::string filename = "";

    if (argc >= 2) {
        filename = argv[1];
    }

    try {
        gavrilova::startCommandInterface(filename, std::cin);
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
