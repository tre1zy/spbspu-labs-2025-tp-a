#include "polygon.hpp"
#include "commands.hpp"
#include <functional>
#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
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
        if (!file) {
            file.clear();
            file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        }
        std::copy(inputIterator(file), inputIterator(), std::back_inserter(polygons));
    }
    std::map< std::string, std::function< void() > > cmds;
    cmds["AREA"] = std::bind(klimova::area, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
    cmds["MAX"] = std::bind(klimova::max, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
    cmds["MIN"] = std::bind(klimova::min, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
    cmds["COUNT"] = std::bind(klimova::count, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
    cmds["PERMS"] = std::bind(klimova::perms, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
    cmds["RECTS"] = std::bind(klimova::rects, std::cref(polygons), std::ref(std::cout));

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
