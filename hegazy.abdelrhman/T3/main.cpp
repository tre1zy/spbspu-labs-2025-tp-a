#include <iostream>
#include <fstream>
#include <iterator>
#include <limits>
#include <map>
#include <functional>
#include <algorithm>
#include <string>
#include "geometry.hpp"
#include "commands.hpp"

int main(int argc, char* argv[])
{
    using Polygon = geom::Polygon;

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::vector<Polygon> polyList;
    std::ifstream inFile(argv[1]);

    if (!inFile) {
        std::cerr << "Error opening file: " << argv[1] << "\n";
        return 1;
    }

    while (!inFile.eof()) {
        Polygon poly;
        if (inFile >> poly) {
            polyList.push_back(poly);
        }
        else {
            inFile.clear();
            inFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    using CommandHandler = std::function<void()>;
    std::map<std::string, CommandHandler> commandMap;

    commandMap["AREA"] = [&]() {
        bob::printAreaSum(std::cin, polyList, std::cout);
        };
    commandMap["MAX"] = [&]() {
        bob::printMaxValueOf(std::cin, polyList, std::cout);
        };
    commandMap["MIN"] = [&]() {
        bob::printMinValueOf(std::cin, polyList, std::cout);
        };
    commandMap["COUNT"] = [&]() {
        bob::printCountOf(std::cin, polyList, std::cout);
        };
    commandMap["LESSAREA"] = [&]() {
        bob::printLessAreaCnt(std::cin, polyList, std::cout);
        };
    commandMap["INTERSECTIONS"] = [&]() {
        bob::printIntersectionsCnt(std::cin, polyList, std::cout);
        };

    std::string line;
    while (std::cin >> line) {
        try {
            auto it = commandMap.find(line);
            if (it != commandMap.end()) {
                it->second();
                std::cout << "\n";
            }
            else {
                throw std::invalid_argument("Unknown command");
            }
        }
        catch (const std::exception& e) {
            std::cout << "<INVALID COMMAND>\n";
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }
    return 0;
}
