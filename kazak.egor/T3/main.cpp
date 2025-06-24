#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <functional>
#include <map>
#include <cstdlib>
#include <limits>
#include "polygon.hpp"
#include "commands.hpp"

static bool readPolygonsFromFile(const std::string & filename, std::vector<Polygon> & polygons) {
  std::ifstream ifs(filename.c_str());
  if (!ifs)
    return false;

  std::string line;
  while (std::getline(ifs, line)) {
    Polygon poly;
    if (parsePolygon(line, poly)) {
      polygons.push_back(poly);
    }
  }
  return true;
}

static void printInvalidCommand() {
  std::cout << "<INVALID COMMAND>\n";
}

int main(int argc, char * argv[]) {
  if (argc != 2) {
    std::cerr << "Error: filename argument missing\n";
    return 1;
  }
  std::vector<Polygon> polygons;
  if (!readPolygonsFromFile(argv[1], polygons)) {
    std::cerr << "Error: could not open or read file\n";
    return 2;
  }

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty())
      continue;
    std::istringstream iss(line);
    std::string cmd;
    iss >> cmd;
    if (cmd == "AREA") {
      std::string param;
      iss >> param;
      if (!iss) {
        printInvalidCommand();
        continue;
      }
      commands::commandArea(polygons, param, std::cout);
    } else if (cmd == "MAX") {
      std::string param;
      iss >> param;
      if (!iss) {
        printInvalidCommand();
        continue;
      }
      commands::commandMax(polygons, param, std::cout);
    } else if (cmd == "MIN") {
      std::string param;
      iss >> param;
      if (!iss) {
        printInvalidCommand();
        continue;
      }
      commands::commandMin(polygons, param, std::cout);
    } else if (cmd == "COUNT") {
      std::string param;
      iss >> param;
      if (!iss) {
        printInvalidCommand();
        continue;
      }
      commands::commandCount(polygons, param, std::cout);
    } else if (cmd == "ECHO") {
      std::string rest;
      std::getline(iss, rest);
      if (rest.empty()) {
        printInvalidCommand();
        continue;
      }

      size_t pos = rest.find_first_not_of(" ");
      if (pos == std::string::npos) {
        printInvalidCommand();
        continue;
      }
      rest = rest.substr(pos);
      commands::commandEcho(polygons, rest, std::cout);
    } else if (cmd == "INFRAME") {
      std::string rest;
      std::getline(iss, rest);
      if (rest.empty()) {
        printInvalidCommand();
        continue;
      }
      size_t pos = rest.find_first_not_of(" ");
      if (pos == std::string::npos) {
        printInvalidCommand();
        continue;
      }
      rest = rest.substr(pos);
      commands::commandInframe(polygons, rest, std::cout);
    } else {
      printInvalidCommand();
    }
  }

  return 0;
}
