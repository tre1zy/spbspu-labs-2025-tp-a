#include "polygon.h"
#include <functional>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

int main(int argc, char* argv[])
{
    using klimova::Polygon;
    using inputIterator = std::istream_iterator< Polygon >;

    if (argc != 2) {
        return 1;
    }
    std::ifstream file(argv[1]);
    std::vector< Polygon > polygons;
    while (!file.eof()) {
        if (!file)
        {
            file.clear();
            file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        }
        std::copy(inputIterator(file), inputIterator(), std::back_inserter(polygons));
    }
    std::map< std::string, std::function< void() > > cmds;
    //cmds["area"] = std::bind(klimova::area, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
    std::string command;
    while (!(std::cin >> command).eof()) {
        try {
            cmds.at(command)();
        }
        catch (...) {
            if (std::cin.fail()) {
                std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
            }
            std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
            std::cout << "<INVALID COMMAND>\n";
        }
    }
    return 0;
}
